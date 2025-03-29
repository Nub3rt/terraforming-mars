// Based on: https://learnopengl.com/In-Practice/Text-Rendering

#pragma once

#include <map>
#include <string>

#include <glm/glm.hpp>

#include <GL/glew.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace view
{
struct Character
{
    GLuint texture_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    FT_Pos advance;
};

class TextRenderer
{
public:
    static bool Init( std::string font, FT_UInt font_size );
    static bool LoadFont( std::string font, FT_UInt font_size );
    static void Resize( int w, int h );
    static void Clean();

    static void RenderText( std::string text, float x, float y, float scale, glm::vec4 color = glm::vec4( 1.0f ) );
    static void RenderTextCentered( std::string text, float x, float y, float scale, glm::vec4 color = glm::vec4( 1.0f ) );

    inline static void RenderText( std::string text, float x, float y, float scale, glm::vec3 color ) { RenderText( text, x, y, scale, glm::vec4( color, 1.0f ) ); }
    inline static void RenderTextCentered( std::string text, float x, float y, float scale, glm::vec3 color ) { RenderTextCentered( text, x, y, scale, glm::vec4( color, 1.0f ) ); }

private:
    TextRenderer();

    static GLuint _vao_id;
    static GLuint _vbo_id;
    static GLuint _program_id;

    static int _width;
    static int _height;

    static std::map<char, Character> _characters;

    static void DoRenderText( std::string text, float x, float y, float scale, glm::vec4 color );
};
}
