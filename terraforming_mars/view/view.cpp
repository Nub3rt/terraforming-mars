#include "view.h"

#include <glm/glm.hpp>

#include "constants.h"

namespace view
{
View::View() {}
View::~View() {}

bool View::Init( Camera* camera, model::GameModel* model ) {
    InitShaders();
    InitGeometry();
    InitTextures();


    _camera = camera;
    _camera_manipulator = new SphericalCameraManipulator();
    _camera_manipulator->SetCamera( camera );

    _model = model;
    _model->Start();

    _tiles.clear();
    for ( const model::boards::Tile& tile : *_model->get_board() ) {
        _tiles.emplace_back( tile );
    }
    _starting_card_id = (int)_tiles.size();

    return true;
}

void View::Clean() {
    CleanShaders();
    CleanGeometry();
    CleanTextures();

    delete _camera_manipulator;

    delete _model;
}

void View::Update( const UpdateInfo& update_info ) {
    _elapsed = update_info.elapsed;

    _camera_manipulator->Update( update_info.delta );
}

void View::Render() {
    RenderBoard();
}

void View::RenderGUI() {
}

void View::KeyboardDown( const SDL_KeyboardEvent& key ) {
    _camera_manipulator->KeyboardDown( key );
}

void View::KeyboardUp( const SDL_KeyboardEvent& key ) {
    _camera_manipulator->KeyboardUp( key );
}

void View::MouseMotion( const SDL_MouseMotionEvent& mouse ) {
    _camera_manipulator->MouseMove( mouse );
}

void View::MouseDown( const SDL_MouseButtonEvent& mouse ) {
    uint8_t id;
    glReadPixels( (GLint)mouse.x, _height - (GLint)mouse.y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &id );
}

void View::MouseUp( const SDL_MouseButtonEvent& mouse ) {
}

void View::MouseWheel( const SDL_MouseWheelEvent& wheel ) {
    _camera_manipulator->MouseWheel( wheel );
}

void View::Resize( int w, int h ) {
    _width = w;
    _height = h;
}

void View::OtherEvent( const SDL_Event& event ) {
}

void View::RenderBoard() {
    glUseProgram( _program_id );
    glBindVertexArray( _hexagon_gpu.vao_id );

    for ( TileWrapper& tile : _tiles ) {
        RenderHexagon( tile );
    }

    glBindVertexArray( 0 );
    glUseProgram( 0 );
}

void View::RenderHexagon( TileWrapper& tile ) {
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, _orange_texture_id );

    /*
    *  *----> q         Ʌ y
    *   \               |
    *    \      ----->  |
    *     \             |
    *      V r          *----> x
    */

    auto& [q_o, r_o] = GetBoardOrigin();
    float q = tile.tile.q - q_o;
    float r = tile.tile.r - r_o;

    float x = glm::root_three<float>() * q + glm::root_three<float>() / 2.0f * r;
    float y = 3.0f / 2.0f * -r;
    glm::mat4 world = glm::translate( glm::vec3( x, y, 0.0f ) );

    glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );
    glUniformMatrix4fv( ul( "world_it" ), 1, GL_FALSE, glm::value_ptr( glm::transpose( glm::inverse( world ) ) ) );
    glUniformMatrix4fv( ul( "view_proj" ), 1, GL_FALSE, glm::value_ptr( _camera->GetViewProj() ) );

    glUniform1i( ul( "color" ), 0 );

    glStencilFunc( GL_ALWAYS, q + 9 * r, 0xff );

    glDrawElements( GL_TRIANGLES, _hexagon_gpu.count, GL_UNSIGNED_INT, nullptr );

    glBindTexture( GL_TEXTURE_2D, 0 );
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
    MeshObject<VertexF> hexagon_cpu;

    glm::vec3 position( 0.0f, 1.0f, 0.0f );
    glm::vec3 normal( 0.0f, 0.0f, 1.0f );
    glm::vec2 texcoord( 0.0f, 0.0f );
    float on_edge = 1.0f;
    hexagon_cpu.vertex_array.emplace_back( glm::vec3( 0.0f ), normal, texcoord, 0.0f );

    for ( int i = 0; i < 6; ++i ) {
        hexagon_cpu.vertex_array.emplace_back( position, normal, texcoord, on_edge );

        static const glm::mat4 rotate_sixth = glm::rotate( glm::pi<float>() / 3.0f, glm::vec3( 0.0f, 0.0f, 1.0f ) );
        position = (rotate_sixth * glm::vec4( position, 1.0f )).xyz;
    }
    for ( int i = 1; i <= 6; ++i ) {
        hexagon_cpu.index_array.push_back( 0 );
        hexagon_cpu.index_array.push_back( i );
        hexagon_cpu.index_array.push_back( i % 6 + 1 );
    }

    _hexagon_gpu = CreateGLObjectFromMesh( hexagon_cpu, _vertex_plus_attribute_list );
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

const std::pair<float, float>& View::GetBoardOrigin() {
    static const std::pair<float, float> origin( 4.0f, 4.0f );
    return origin;
}

const std::initializer_list<VertexAttributeDescriptor> View::_vertex_attribute_list =
{
    { 0, offsetof( Vertex, position ), 3, GL_FLOAT },
    { 1, offsetof( Vertex, normal   ), 3, GL_FLOAT },
    { 2, offsetof( Vertex, texcoord ), 2, GL_FLOAT },  
};

const std::initializer_list<VertexAttributeDescriptor> View::_vertex_plus_attribute_list =
{
    { 0, offsetof( VertexF, position ), 3, GL_FLOAT },
    { 1, offsetof( VertexF, normal   ), 3, GL_FLOAT },
    { 2, offsetof( VertexF, texcoord ), 2, GL_FLOAT },
    { 3, offsetof( VertexF, plus     ), 1, GL_FLOAT },
};
}
