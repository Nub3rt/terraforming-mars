#include "view.h"

#include <glm/glm.hpp>

namespace view
{
View::View() {}
View::~View() {}

bool View::Init( Camera* camera) {
    _camera = camera;

    InitShaders();
    InitGeometry();
    InitTextures();

    return true;
}

void View::Clean() {
    CleanShaders();
    CleanGeometry();
    CleanTextures();
}

void View::Update( const UpdateInfo& update_info ) {
}

void View::Render() {
    glUseProgram( _program_id );
    glBindVertexArray( _hexagon_gpu.vao_id );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, _orange_texture_id );

    glm::mat4 world = glm::identity<glm::mat4>();

    glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );
    glUniformMatrix4fv( ul( "world_it" ), 1, GL_FALSE, glm::value_ptr( glm::transpose( glm::inverse( world ) ) ) );
    glUniformMatrix4fv( ul( "view_proj" ), 1, GL_FALSE, glm::value_ptr( _camera->GetViewProj() ) );

    glUniform1i( ul( "color" ), 0 );

    glDrawElements( GL_TRIANGLES, _hexagon_gpu.count, GL_UNSIGNED_INT, nullptr );

    glBindTexture( GL_TEXTURE_2D, 0 );
    glBindVertexArray( 0 );
    glUseProgram( 0 );
}

void View::RenderGUI() {
}
void View::KeyboardDown( const SDL_KeyboardEvent& key ) {
}
void View::KeyboardUp( const SDL_KeyboardEvent& key ) {
}
void View::MouseMotion( const SDL_MouseMotionEvent& mouse ) {
}
void View::MouseDown( const SDL_MouseButtonEvent& mouse ) {
}
void View::MouseUp( const SDL_MouseButtonEvent& mouse ) {
}
void View::MouseWheel( const SDL_MouseWheelEvent& wheel ) {
}
void View::Resize( int w, int h ) {
}
void View::OtherEvent( const SDL_Event& event ) {
}

void View::InitShaders() {
    _program_id = glCreateProgram();
    AttachShader( _program_id, GL_VERTEX_SHADER, "shaders/pos_norm_tex.vert" );
    AttachShader( _program_id, GL_FRAGMENT_SHADER, "shaders/lighting.frag" );
    LinkProgram( _program_id );
}

void View::CleanShaders() {
    glDeleteProgram( _program_id );
}

void View::InitGeometry() {
    MeshObject<Vertex> hexagon_cpu;

    glm::vec3 position( 1.0f, 0.0f, 0.0f );
    glm::vec3 normal( 0.0f, 1.0f, 0.0f );
    glm::vec2 texcoord( 0.0f, 0.0f );
    hexagon_cpu.vertex_array.emplace_back( glm::vec3( 0.0f ), normal, texcoord );

    for ( int i = 0; i < 6; ++i ) {
        hexagon_cpu.vertex_array.emplace_back( position, normal, texcoord );

        static const glm::mat4 rotate_sixth = glm::rotate( glm::pi<float>() / 3.0f, glm::vec3( 0.0f, 1.0f, 0.0f ) );
        position = (rotate_sixth * glm::vec4( position, 1.0f )).xyz;
    }
    for ( int i = 1; i <= 6; ++i ) {
        hexagon_cpu.index_array.push_back( 0 );
        hexagon_cpu.index_array.push_back( i );
        hexagon_cpu.index_array.push_back( i % 6 + 1 );
    }

    _hexagon_gpu = CreateGLObjectFromMesh( hexagon_cpu, _vertex_attribute_list );
}

void View::CleanGeometry() {
    CleanOGLObject( _hexagon_gpu );
}

void View::InitTextures() {
    glGenTextures( 1, &_orange_texture_id );
    glBindTexture( GL_TEXTURE_2D, _orange_texture_id );
    
    unsigned char data[ 3 ] = { 0xff, 0x55, 0x55 };

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glBindTexture( GL_TEXTURE_2D, 0 );
}

void View::CleanTextures() {
    glDeleteTextures( 1, &_orange_texture_id );
}

const std::initializer_list<VertexAttributeDescriptor> View::_vertex_attribute_list =
{
    { 0, offsetof( Vertex, position ), 3, GL_FLOAT },
    { 1, offsetof( Vertex, normal ), 3, GL_FLOAT },
    { 2, offsetof( Vertex, texcoord ), 2, GL_FLOAT },
};
}
