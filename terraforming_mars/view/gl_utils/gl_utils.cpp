#include "gl_utils.hpp"

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <SDL3_image/SDL_image.h>

namespace view
{
static void loadShaderCode( std::string& shader_code, const std::filesystem::path& filename ) {
    shader_code = "";

    std::ifstream shaderStream( filename );
    if ( !shaderStream.is_open() ) {
        SDL_LogMessage( SDL_LOG_CATEGORY_ERROR,
                        SDL_LOG_PRIORITY_ERROR,
                        "Error while opening shader code file %s!", filename.string().c_str() );
        return;
    }

    std::string line = "";
    while ( std::getline( shaderStream, line ) ) {
        shader_code += line + "\n";
    }

    shaderStream.close();
}

GLuint AttachShader( const GLuint program_id, GLenum shader_type, const std::filesystem::path& filename ) {
    std::string shader_code;
    loadShaderCode( shader_code, filename );

    return AttachShaderCode( program_id, shader_type, shader_code );
}

GLuint AttachShaderCode( const GLuint program_id, GLenum shader_type, std::string_view shader_code ) {
    if ( program_id == 0 ) {
        SDL_LogMessage( SDL_LOG_CATEGORY_ERROR,
                        SDL_LOG_PRIORITY_ERROR,
                        "Program needs to be initialized before loading!" );
        return 0;
    }

    GLuint shader_id = glCreateShader( shader_type );

    const char* source_pointer = shader_code.data();
    GLint source_length = static_cast<GLint>( shader_code.length() );

    glShaderSource( shader_id, 1, &source_pointer, &source_length );

    glCompileShader( shader_id );

    GLint result = GL_FALSE;
    int info_log_length;

    glGetShaderiv( shader_id, GL_COMPILE_STATUS, &result );
    glGetShaderiv( shader_id, GL_INFO_LOG_LENGTH, &info_log_length );

    if ( GL_FALSE == result || info_log_length != 0 ) {
        std::string error_message( info_log_length, '\0' );
        glGetShaderInfoLog( shader_id, info_log_length, NULL, error_message.data() );

        SDL_LogMessage( SDL_LOG_CATEGORY_ERROR,
                        result ? SDL_LOG_PRIORITY_WARN : SDL_LOG_PRIORITY_ERROR,
                        "[glCompileShader]: %s", error_message.data() );
    }

    glAttachShader( program_id, shader_id );

    return shader_id;
}

void LinkProgram( const GLuint program_id, bool own_shaders ) {
    glLinkProgram( program_id );

    GLint info_log_length = 0, result = 0;

    glGetProgramiv( program_id, GL_LINK_STATUS, &result );
    glGetProgramiv( program_id, GL_INFO_LOG_LENGTH, &info_log_length );
    if ( GL_FALSE == result || info_log_length != 0 ) {
        std::string error_message( info_log_length, '\0' );
        glGetProgramInfoLog( program_id, info_log_length, nullptr, error_message.data() );
        SDL_LogMessage( SDL_LOG_CATEGORY_ERROR,
                        result ? SDL_LOG_PRIORITY_WARN : SDL_LOG_PRIORITY_ERROR,
                        "[glLinkProgram]: %s", error_message.data() );
    }

    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteShader.xhtml
    if ( own_shaders ) {
        GLint attached_shaders = 0;
        glGetProgramiv( program_id, GL_ATTACHED_SHADERS, &attached_shaders );
        std::vector<GLuint> shaders( attached_shaders );

        glGetAttachedShaders( program_id, attached_shaders, nullptr, shaders.data() );

        for ( GLuint shader : shaders ) {
            glDeleteShader( shader );
        }
    }
}

static inline ImageRGBA::TexelRGBA* get_image_row( ImageRGBA& image, int rowIndex ) {
    return &image.texel_data[ rowIndex * image.width ];
}

static void invert_image_RGBA( ImageRGBA& image ) {
    int height_div_2 = image.height / 2;


    for ( int index = 0; index < height_div_2; index++ ) {
        std::uint32_t* lower_data = reinterpret_cast<std::uint32_t*>( get_image_row( image, index ) );
        std::uint32_t* higher_data = reinterpret_cast<std::uint32_t*>( get_image_row( image, image.height - 1 - index ) );

        for ( unsigned int rowIndex = 0; rowIndex < image.width; rowIndex++ ) {
            lower_data[ rowIndex ] ^= higher_data[ rowIndex ];
            higher_data[ rowIndex ] ^= lower_data[ rowIndex ];
            lower_data[ rowIndex ] ^= higher_data[ rowIndex ];
        }
    }
}

GLsizei NumberOfMIPLevels( const ImageRGBA& image ) {
    GLsizei targetlevel = 1;
    unsigned int index = std::max( image.width, image.height );

    while ( index >>= 1 )
        ++targetlevel;

    return targetlevel;
}

[[nodiscard]] ImageRGBA ImageFromFile( const std::filesystem::path& filename, bool needs_flip ) {
    ImageRGBA img;

    std::unique_ptr<SDL_Surface, decltype( &SDL_DestroySurface )> loaded_img( IMG_Load( filename.string().c_str() ), SDL_DestroySurface );
    if ( !loaded_img ) {
        SDL_LogMessage( SDL_LOG_CATEGORY_ERROR,
                        SDL_LOG_PRIORITY_ERROR,
                        "[ImageFromFile] Error while loading image file: %s", filename.string().c_str() );
        return img;
    }

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    SDL_PixelFormat format = SDL_PIXELFORMAT_ABGR8888;
#else
    SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA8888;
#endif

    std::unique_ptr<SDL_Surface, decltype( &SDL_DestroySurface )> formatted_surface( SDL_ConvertSurface( loaded_img.get(), format ), SDL_DestroySurface );

    if ( !formatted_surface ) {
        SDL_LogMessage( SDL_LOG_CATEGORY_ERROR,
                        SDL_LOG_PRIORITY_ERROR,
                        "[ImageFromFile] Error while processing texture" );
        return img;
    }

    img.Assign( reinterpret_cast<const std::uint32_t*>(formatted_surface->pixels), formatted_surface->w, formatted_surface->h );

    // SDL coord system ( (0,0) top left ) => OpenGL texture coord system ( (0,0) bottom left )
    if ( needs_flip )
        invert_image_RGBA( img );

    return img;
}

void CleanOGLObject( OGLObject& ObjectGPU ) {
    glDeleteBuffers( 1, &ObjectGPU.vbo_id );
    ObjectGPU.vbo_id = 0;
    glDeleteBuffers( 1, &ObjectGPU.ibo_id );
    ObjectGPU.ibo_id = 0;
    glDeleteVertexArrays( 1, &ObjectGPU.vao_id );
    ObjectGPU.vao_id = 0;
}
}
