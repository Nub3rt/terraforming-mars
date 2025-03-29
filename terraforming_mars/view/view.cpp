#include "view.h"

#include <iostream>

#include <glm/glm.hpp>
#include <imgui.h>

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

    glClear( GL_DEPTH_BUFFER_BIT );

    RenderHUD();
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
    std::cout << std::to_string( id ) << std::endl;
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

    for ( int i = 0; i < _tiles.size(); ++i) {
        RenderHexagon( _tiles[i], i );
    }

    glBindVertexArray( 0 );
    glUseProgram( 0 );
}

void View::RenderHexagon( TileWrapper& tile, int id ) {
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
    float q = tile->q - q_o;
    float r = tile->r - r_o;

    float x = glm::root_three<float>() * q + glm::root_three<float>() / 2.0f * r;
    float y = 3.0f / 2.0f * -r;
    glm::mat4 world = glm::translate( glm::vec3( x, y, 0.0f ) );

    glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );
    glUniformMatrix4fv( ul( "world_it" ), 1, GL_FALSE, glm::value_ptr( glm::transpose( glm::inverse( world ) ) ) );
    glUniformMatrix4fv( ul( "view_proj" ), 1, GL_FALSE, glm::value_ptr( _camera->GetViewProj() ) );

    glUniform1i( ul( "color" ), 0 );

    SetStencilRef( id );

    glDrawElements( GL_TRIANGLES, _hexagon_gpu.count, GL_UNSIGNED_INT, nullptr );

    SetStencilRef();

    glBindTexture( GL_TEXTURE_2D, 0 );
}

void View::RenderHUD() {
    RenderHand();
}

void View::RenderHand() {
    glUseProgram( _program_sprite_sheet_id );
    glBindVertexArray( _rectangle_gpu.vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, _cards_texture_id );
    glUniform1i( ul( "image" ), 0 );

    CardWrapper card( _model->get_local_player()->get_hand()[ 0 ] );
    RenderCard( card, 0 );

    glBindTexture( GL_TEXTURE_2D, 0 );

    glBindVertexArray( 0 );
    glUseProgram( 0 );
}

void View::RenderCard( CardWrapper& card, int index ) {
    static const float stride_x = 1.0f / CARD_TEXTURE_COLUMNS;
    static const float stride_y = 1.0f / CARD_TEXTURE_ROWS;
    int corrected_card_id = +card->get_card_id() - 1;
    int index_x = corrected_card_id % CARD_TEXTURE_COLUMNS;
    int index_y = corrected_card_id / CARD_TEXTURE_COLUMNS;

    static const float card_ratio = (float)CARD_TEXTURE_WIDTH / CARD_TEXTURE_HEIGHT;
    float card_width = card_ratio / _width * _height;
    glm::vec3 scale( card.scale.x * card_width, card.scale.y, 1.0f );
    glm::vec3 translate( card.pos, index / 20.0f );

    glm::mat4 world = glm::translate( translate ) * glm::scale( scale );
    glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

    glUniform1f( ul( "stride_x" ), stride_x );
    glUniform1f( ul( "stride_y" ), stride_y );
    glUniform1i( ul( "index_x" ), index_x );
    glUniform1i( ul( "index_y" ), index_y );

    SetStencilRef( _starting_card_id + index );

    glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );

    SetStencilRef();
}

void View::InitShaders() {
    _program_id = glCreateProgram();
    AttachShader( _program_id, GL_VERTEX_SHADER, "shaders/pos_norm_tex.vert" );
    AttachShader( _program_id, GL_FRAGMENT_SHADER, "shaders/lighting.frag" );
    LinkProgram( _program_id );

    _program_sprite_sheet_id = glCreateProgram();
    AttachShader( _program_sprite_sheet_id, GL_VERTEX_SHADER, "shaders/sprite_sheet.vert" );
    AttachShader( _program_sprite_sheet_id, GL_FRAGMENT_SHADER, "shaders/sprite_sheet.frag" );
    LinkProgram( _program_sprite_sheet_id );
}

void View::CleanShaders() {
    glDeleteProgram( _program_id );
    glDeleteProgram( _program_sprite_sheet_id );
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


    MeshObject<VertexPosTex> rectangle_cpu = {
        std::vector<VertexPosTex> {
            { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
            { {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
            { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f } },
            { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f } },
        },
        std::vector<GLuint> {
            0, 1, 2,
            2, 1, 3,
        }
    };

    _rectangle_gpu = CreateGLObjectFromMesh( rectangle_cpu, _vertex_pos_tex_attribute_list );
}

void View::CleanGeometry() {
    CleanOGLObject( _hexagon_gpu );
    CleanOGLObject( _rectangle_gpu );
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


    ImageRGBA cards = ImageFromFile( "assets/cards.png" );

    glGenTextures( 1, &_cards_texture_id );
    glBindTexture( GL_TEXTURE_2D, _cards_texture_id );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, cards.width, cards.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, cards.data() );
    glGenerateMipmap( GL_TEXTURE_2D );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );


    glBindTexture( GL_TEXTURE_2D, 0 );
}

void View::CleanTextures() {
    glDeleteTextures( 1, &_orange_texture_id );
    glDeleteTextures( 1, &_cards_texture_id );
}

const std::pair<float, float>& View::GetBoardOrigin() {
    static const std::pair<float, float> origin( 4.0f, 4.0f );
    return origin;
}

const std::initializer_list<VertexAttributeDescriptor> View::_vertex_pos_tex_attribute_list =
{
    { 0, offsetof( VertexPosTex, position ), 3, GL_FLOAT },
    { 1, offsetof( VertexPosTex, texcoord ), 2, GL_FLOAT },
};

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
