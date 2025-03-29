// Based on: https://learnopengl.com/In-Practice/Text-Rendering

#include "text_renderer.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <GL/glew.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "constants.h"
#include "gl_utils/gl_utils.h"

namespace view
{
bool TextRenderer::Init( std::string font, FT_UInt font_size ) {
    _program_id = glCreateProgram();
    AttachShader( _program_id, GL_VERTEX_SHADER, "shaders/text.vert" );
    AttachShader( _program_id, GL_FRAGMENT_SHADER, "shaders/text.frag" );
    LinkProgram( _program_id );


    glGenVertexArrays( 1, &_vao_id );
    glGenBuffers( 1, &_vbo_id );

    glBindVertexArray( _vao_id );
    glBindBuffer( GL_ARRAY_BUFFER, _vbo_id );

    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 6 * 4, nullptr, GL_DYNAMIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), 0 );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

    return LoadFont( font, font_size );
}

void TextRenderer::Resize( int w, int h ) {
    _width = w;
    _height = h;
}

void TextRenderer::Clean() {
    glDeleteProgram( _program_id );

    for ( auto& [_, c] : _characters )
        glDeleteTextures( 1, &c.texture_id );

    glDeleteBuffers( 1, &_vbo_id );
    glDeleteBuffers( 1, &_vao_id );
}

bool TextRenderer::LoadFont( std::string font, FT_UInt font_size ) {
    for ( auto& [_, c] : _characters )
        glDeleteTextures( 1, &c.texture_id );

    _characters.clear();

    FT_Library ft;
    if ( FT_Init_FreeType( &ft ) ) {
        std::cerr << "TextRenderer::LoadFont: Could not init FreeType library!" << std::endl;
        return false;
    }

    FT_Face face;
    if ( FT_New_Face( ft, font.c_str(), 0, &face) ) {
        std::cerr << "TextRenderer::LoadFont: Failed to load font: " << font << std::endl;
        return false;
    }

    FT_Set_Pixel_Sizes( face, 0, font_size );
    GLint unpack_alignment_value;
    glGetIntegerv( GL_UNPACK_ALIGNMENT, &unpack_alignment_value );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    for ( GLubyte c = 0; c < 128; ++c ) {
        if ( FT_Load_Char( face, c, FT_LOAD_RENDER ) ) {
            std::cerr << "TextRenderer::LoadFont: Failed to load glyph for char with id: " << c << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures( 1, &texture );
        glBindTexture( GL_TEXTURE_2D, texture );
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        Character character = {
            texture,
            { face->glyph->bitmap.width, face->glyph->bitmap.rows },
            { face->glyph->bitmap_left, face->glyph->bitmap_top },
            face->glyph->advance.x
        };
        _characters.emplace( c, std::move(character) );
    }

    glBindTexture( GL_TEXTURE_2D, 0 );

    FT_Done_Face( face );
    FT_Done_FreeType( ft );

    glPixelStorei( GL_UNPACK_ALIGNMENT, unpack_alignment_value );

    return true;
}

void TextRenderer::RenderText( std::string text, float x, float y, float scale, glm::vec4 color ) {
    x = x * 2.0f - 1.0f;
    y = y * 2.0f - 1.0f;
    scale *= (float)_width / WINDOW_WIDTH;

    DoRenderText( text, x, y, scale, color );
}

void TextRenderer::RenderTextCentered( std::string text, float x, float y, float scale, glm::vec4 color ) {
    x = x * 2.0f - 1.0f;
    y = y * 2.0f - 1.0f;
    scale *= (float)_width / WINDOW_WIDTH;
    float advance = 0.0f;
    float max_h = 0.0f;

    for ( auto ch : text ) {
        Character c = _characters.contains( ch ) ? _characters[ ch ] : _characters[ '?' ];

        advance += (c.advance >> 6) * scale / _width;

        float h = c.size.y * scale / _height;
        if ( h > max_h )
            max_h = h;
    }

    DoRenderText( text, x - advance / 2.0f, y - max_h / 2.0f, scale, color );
}

void TextRenderer::DoRenderText( std::string text, float x, float y, float scale, glm::vec4 color ) {
    GLboolean depth_test_enabled;
    glGetBooleanv( GL_DEPTH_TEST, &depth_test_enabled );

    GLboolean stencil_test_enabled;
    glGetBooleanv( GL_STENCIL_TEST, &stencil_test_enabled );

    GLboolean blend_enabled;
    glGetBooleanv( GL_BLEND, &blend_enabled );

    GLint src_blend, dst_blend;
    glGetIntegerv( GL_BLEND_SRC_ALPHA, &src_blend );
    glGetIntegerv( GL_BLEND_DST_ALPHA, &dst_blend );

    if ( stencil_test_enabled )
        glDisable( GL_STENCIL_TEST );
    if ( depth_test_enabled )
        glDisable( GL_DEPTH_TEST );
    if ( !blend_enabled )
        glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glUseProgram( _program_id );
    glBindVertexArray( _vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glUniform1i( ul( "text" ), 0 );

    glUniform4f( ul( "text_col" ), color.r, color.g, color.b, color.a );

    for ( auto& ch : text ) {
        Character c = _characters.contains( ch ) ? _characters[ ch ] : _characters[ '?' ];

        float x_pos = x + c.bearing.x * scale / _width;
        float y_pos = y + (c.bearing.y - c.size.y) * scale / _height;

        float w = c.size.x * scale / _width;
        float h = c.size.y * scale / _height;

        float vertices[ 6 ][ 4 ] = {
            { x_pos,     y_pos + h, 0.0f, 0.0f },
            { x_pos,     y_pos,     0.0f, 1.0f },
            { x_pos + w, y_pos,     1.0f, 1.0f },

            { x_pos,     y_pos + h, 0.0f, 0.0f },
            { x_pos + w, y_pos,     1.0f, 1.0f },
            { x_pos + w, y_pos + h, 1.0f, 0.0f },
        };

        glBindTexture( GL_TEXTURE_2D, c.texture_id );

        glBindBuffer( GL_ARRAY_BUFFER, _vbo_id );
        glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( vertices ), vertices );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        glDrawArrays( GL_TRIANGLES, 0, 6 );

        x += (c.advance >> 6) * scale / _width;
    }

    glBindVertexArray( 0 );
    glUseProgram( 0 );

    if ( !blend_enabled )
        glDisable( GL_BLEND );
    glBlendFunc( src_blend, dst_blend );

    if ( depth_test_enabled )
        glEnable( GL_DEPTH_TEST );
    if ( stencil_test_enabled )
        glEnable( GL_STENCIL_TEST );
}

GLuint TextRenderer::_vao_id = 0;
GLuint TextRenderer::_vbo_id = 0;
GLuint TextRenderer::_program_id = 0;
int TextRenderer::_width = 0;
int TextRenderer::_height = 0;
std::map<char, Character> TextRenderer::_characters;
}
