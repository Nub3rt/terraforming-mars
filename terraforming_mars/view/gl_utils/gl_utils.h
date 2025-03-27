// Based on http://www.opengl-tutorial.org/

#pragma once

#include <filesystem>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace view
{
struct UpdateInfo
{
    float elapsed = 0.0f;
    float delta = 0.0f;
};

struct VertexPosColor
{
    glm::vec3 position;
    glm::vec3 color;
};

struct VertexPosTex
{
    glm::vec3 position;
    glm::vec2 texcoord;
};

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
};

struct VertexF
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
    GLfloat plus;
};

struct ImageRGBA
{
    typedef glm::u8vec4 TexelRGBA;

    static_assert( sizeof( TexelRGBA ) == sizeof( std::uint32_t ) );


    std::vector<TexelRGBA> texel_data;
    unsigned int width = 0;
    unsigned int height = 0;

    bool Allocate( unsigned int width, unsigned int height ) {
        this->width = width;
        this->height = height;

        texel_data.resize( width * height );

        return !texel_data.empty();
    }

    bool Assign( const std::uint32_t* texel_data, unsigned int width, unsigned int height ) {
        this->width = width;
        this->height = height;

        const TexelRGBA* _data = reinterpret_cast<const TexelRGBA*>( texel_data );

        this->texel_data.assign( _data, _data + width * height );

        return !this->texel_data.empty();
    }

    TexelRGBA GetTexel( unsigned int x, unsigned int y ) const {
        return texel_data[ y * width + x ];
    }

    void SetTexel( unsigned int x, unsigned int y, const TexelRGBA& texel ) {
        texel_data[ y * width + x ] = texel;
    }

    const TexelRGBA* data() const {
        return texel_data.data();
    }
};

template<typename VertexT>
struct MeshObject
{
    std::vector<VertexT> vertex_array;
    std::vector<GLuint>  index_array;
};

struct OGLObject
{
    GLuint  vao_id = 0; // vertex array object resource id
    GLuint  vbo_id = 0; // vertex buffer object resource id
    GLuint  ibo_id = 0; // index buffer object resource id
    GLsizei count = 0; // number of index/vertex to draw
};

struct VertexAttributeDescriptor
{
    GLuint index = -1;
    GLuint stride_in_bytes = 0;
    GLint  number_of_components = 0;
    GLenum gl_type = GL_NONE;
};


GLuint AttachShader( const GLuint program_id, GLenum shader_type, const std::filesystem::path& filename );
GLuint AttachShaderCode( const GLuint program_id, GLenum shader_type, std::string_view shader_code );
void LinkProgram( const GLuint program_id, bool own_shaders = true );


template <typename VertexT>
[[nodiscard]] OGLObject CreateGLObjectFromMesh( const MeshObject<VertexT>& mesh, std::initializer_list<VertexAttributeDescriptor> vertex_attr_desc_list ) {
    OGLObject mesh_gpu = { 0 };

    glCreateBuffers( 1, &mesh_gpu.vbo_id );

    glNamedBufferData( mesh_gpu.vbo_id,
                       mesh.vertex_array.size() * sizeof( VertexT ),
                       mesh.vertex_array.data(),
                       GL_STATIC_DRAW );

    glCreateBuffers( 1, &mesh_gpu.ibo_id );
    glNamedBufferData( mesh_gpu.ibo_id, mesh.index_array.size() * sizeof( GLuint ), mesh.index_array.data(), GL_STATIC_DRAW );

    mesh_gpu.count = static_cast<GLsizei>( mesh.index_array.size() );

    glCreateVertexArrays( 1, &mesh_gpu.vao_id );

    glVertexArrayVertexBuffer( mesh_gpu.vao_id, 0, mesh_gpu.vbo_id, 0, sizeof( VertexT ) );

    for ( const auto& vertexAttrDesc : vertex_attr_desc_list ) {
        glEnableVertexArrayAttrib( mesh_gpu.vao_id, vertexAttrDesc.index );
        glVertexArrayAttribBinding( mesh_gpu.vao_id, vertexAttrDesc.index, 0 );

        glVertexArrayAttribFormat(
            mesh_gpu.vao_id,
            vertexAttrDesc.index,
            vertexAttrDesc.number_of_components,
            vertexAttrDesc.gl_type,
            GL_FALSE,
            vertexAttrDesc.stride_in_bytes
        );
    }
    glVertexArrayElementBuffer( mesh_gpu.vao_id, mesh_gpu.ibo_id );

    return mesh_gpu;
}

void CleanOGLObject( OGLObject& ObjectGPU );

[[nodiscard]] ImageRGBA ImageFromFile( const std::filesystem::path& filename, bool needs_flip = true );
GLsizei NumberOfMIPLevels( const ImageRGBA& );

inline GLint ul( GLuint program_id, const GLchar* uniform_name ) noexcept {
    // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetUniformLocation.xhtml
    return glGetUniformLocation( program_id, uniform_name );
}

inline GLint ul( const GLchar* uniform_name ) noexcept {
    GLint program_id;
    glGetIntegerv( GL_CURRENT_PROGRAM, &program_id );
    return ul( program_id, uniform_name );
}
}
