#include "view.h"

#include <array>
#include <iostream>
#include <stdexcept>

#include <glm/glm.hpp>
#include <imgui.h>

#include "constants.h"
#include "text_renderer.h"

#include "../model/constants.h"

namespace view
{
View::View() : _resources(), _resource_productions() {}
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
    _stencil_starting_card = STENCIL_STARTING_BOARD + (int)_tiles.size();

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

#pragma region Events

void View::KeyboardDown( const SDL_KeyboardEvent& key ) {
    _camera_manipulator->KeyboardDown( key );
}

void View::KeyboardUp( const SDL_KeyboardEvent& key ) {
    _camera_manipulator->KeyboardUp( key );
}

void View::MouseMotion( const SDL_MouseMotionEvent& mouse ) {
    _camera_manipulator->MouseMove( mouse );

    uint8_t hovered_id;
    glReadPixels( (GLint)mouse.x, _height - (GLint)mouse.y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &hovered_id );

    float mouse_x = mouse.x / _width * 2.0f - 1.0f;
    float mouse_y = (_height - mouse.y) / _height * 2.0f - 1.0f;
    std::cout << std::format( "start_x: {}, end_x: {}, x: {}\n", _hand_start_x, _hand_end_x, x );

    int hovered_card_index = CalculateHoveredCardByMousePos( mouse_x, mouse_y );

    for ( int i = 0; i < _hand.size(); ++i ) {
        CardWrapper& card = _hand[ i ];
        if ( card.hovered && hovered_card_index != i ) {
            card.hovered = false;
            RefreshHandPositions();
        } else if ( !card.hovered && hovered_card_index == i ) {
            card.hovered = true;
            card.scale *= 2.0f;
            card.pos.y += 0.6f;
            card.rotate = 0.0f;
        }
    }
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

#pragma endregion Events

#pragma region State

void View::ChangeState( ViewState* state ) {
    if ( _state != nullptr )
        delete _state;

    _state = state;
}
#pragma endregion State

void View::RefreshHandPositions() {
    static const float card_ratio = (float)CARD_TEXTURE_WIDTH / CARD_TEXTURE_HEIGHT;
    static const float min_x = -0.6f;
    static const float max_x =  0.6f;
    static const float mid_x = (min_x + max_x) / 2.0f;
    static const float spacing = 0.1f;
    float card_width = CARD_BASE_SCALE.x * card_ratio / _width * _height;

    if ( _hand.size() == 0 )
        return;

    if ( _hand.size() == 1 ) {
        CardWrapper& card = _hand[ 0 ];
        card.pos.x = mid_x;
        card.pos.y = HAND_BASE_Y;
        card.scale = CARD_BASE_SCALE;
        card.rotate = 0.0f;
        return;
    }

    _hand_start_x = fmaxf( min_x, mid_x - (_hand.size() - 1) / 2.0f * spacing );
    _hand_end_x   = fminf( max_x, mid_x + (_hand.size() - 1) / 2.0f * spacing );
    for ( int i = 0; i < _hand.size(); ++i ) {
        CardWrapper& card = _hand[ i ];
        card.pos.x = _hand_start_x + i * spacing;
        if ( card.hovered )
            continue;
        card.pos.y = HAND_BASE_Y;
        card.scale = CARD_BASE_SCALE;
        card.rotate = 0.0f;
    }
}

#pragma region Rendering

void View::RenderBoard() {
    glUseProgram( _program_id );
    glBindVertexArray( _hexagon_gpu.vao_id );

    for ( int i = 0; i < _tiles.size(); ++i) {
        RenderHexagon( _tiles[i], STENCIL_STARTING_BOARD + i );
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
    RenderMenuButton();
    RenderGlobalParameters();
    RenderEndButton();
    RenderResources();
    RenderHand();
}

void View::RenderMenuButton() {
    // TODO
}

void View::RenderGlobalParameters() {
    glUseProgram( _program_rectangle_id );
    glBindVertexArray( _rectangle_gpu.vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glUniform1i( ul( "image" ), 0 );


    std::array<GLuint, 4> to_draw = {
        _temperature_texture.id,
        _ocean_texture.id,
        _oxygen_texture.id,
        _tr_texture.id,
    };
    for ( int i = 0; i < to_draw.size(); ++i ) {
        glBindTexture( GL_TEXTURE_2D, to_draw[ i ] );

        auto [x, y, scale] = CalculateParameterPosition( i, 0 );

        glm::mat4 world = glm::translate( glm::vec3( x, y, 0.0f ) ) * glm::scale( scale );
        glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

        glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );
    }

    glBindTexture( GL_TEXTURE_2D, 0 );

    glBindVertexArray( 0 );
    glUseProgram( 0 );

    std::array<std::pair<int, int>, 4> text_to_draw = {{
        { _temperature, model::MAX_TEMPERATURE },
        { _ocean_count, model::MAX_OCEAN_COUNT },
        { _oxygen_level, model::MAX_OXYGEN_LEVEL },
        { _tr, -1 },
    }};
    for ( int i = 0; i < text_to_draw.size(); ++i ) {
        static const float scale = 1.5f;
        auto& [current, max] = text_to_draw[ i ];
        glm::vec3 color = current == max ? glm::vec3( 0.0f, 1.0f, 0.0f ) : glm::vec3( 1.0f );

        auto [x, y, _] = CalculateParameterPosition( i, 1 );
        TextRenderer::RenderTextCentered(
            std::to_string( current ),
            x,
            y,
            scale,
            color
        );
    }
}

void View::RenderEndButton() {
    static const float button_ratio = (float)_button_texture.height / _temperature_texture.width;
    static const float size = 0.06f;
    static const float spacing = size * 2.5f;
    static const float ratio_modifier = 0.8f;

    float x = 1.0f - size - spacing / button_ratio;
    float y = 0.0f;
    glm::vec3 scale( size * button_ratio, size * _width / _height * ratio_modifier, 1.0f );

    glUseProgram( _program_rectangle_id );
    glBindVertexArray( _rectangle_gpu.vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glUniform1i( ul( "image" ), 0 );

    glBindTexture( GL_TEXTURE_2D, _button_texture.id );

    glm::mat4 world = glm::translate( glm::vec3( x, y, 0.0f ) ) * glm::scale( scale );
    glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

    SetStencilRef( STENCIL_END );

    glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );

    SetStencilRef();

    glBindTexture( GL_TEXTURE_2D, 0 );

    glBindVertexArray( 0 );
    glUseProgram( 0 );

    static const glm::vec3 color = glm::vec3( 1280.f, 58.0f, 47.0f ) / 255.0f;
    TextRenderer::RenderTextCentered(
        "End Generation",
        x,
        y,
        1.0f,
        color
    );
}

void View::RenderResources() {
    glUseProgram( _program_rectangle_id );
    glBindVertexArray( _rectangle_gpu.vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glUniform1i( ul( "image" ), 0 );

    glBindTexture( GL_TEXTURE_2D, _production_box_texture.id );
    for ( int i = 0; i < +model::Resource::MAX; ++i ) {
        auto [x, y, scale] = CalculateResourcePosition( i, 0 );

        glm::mat4 world = glm::translate( glm::vec3( x, y, 0.0f ) ) * glm::scale( scale );
        glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

        glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );
    }


    glUseProgram( _program_sprite_sheet_id );

    glUniform1i( ul( "image" ), 0 );

    glBindTexture( GL_TEXTURE_2D, _resources_texture.id );
    for ( int i = 0; i < +model::Resource::MAX; ++i ) {
        static const float stride_x = 1.0f / RESOURCE_TEXTURE_COLUMNS;
        static const float stride_y = 1.0f / RESOURCE_TEXTURE_ROWS;
        int res_index = +model::Resource::MAX - i - 1;
        int index_x = res_index % RESOURCE_TEXTURE_COLUMNS;
        int index_y = res_index / RESOURCE_TEXTURE_COLUMNS;

        auto [x, y, scale] = CalculateResourcePosition( i, 1 );

        glm::mat4 world = glm::translate( glm::vec3( x, y, 0.0f ) ) * glm::scale( scale );
        glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

        glUniform1f( ul( "stride_x" ), stride_x );
        glUniform1f( ul( "stride_y" ), stride_y );
        glUniform1i( ul( "index_x" ), index_x );
        glUniform1i( ul( "index_y" ), index_y );

        glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );
    }

    glBindTexture( GL_TEXTURE_2D, 0 );

    glBindVertexArray( 0 );
    glUseProgram( 0 );


    for ( int i = 0; i < +model::Resource::MAX; ++i ) {
        static const float scale = 1.5f;

        auto [x, y, _] = CalculateResourcePosition( i, 0 );
        TextRenderer::RenderTextCentered(
            std::to_string( _resource_productions[ i + 1 ] ),
            x,
            y,
            scale,
            glm::vec3( 0.0f )
        );

        std::tie( x, y, _ ) = CalculateResourcePosition( i, 2 );
        TextRenderer::RenderTextCentered(
            std::to_string( _resources[ i + 1 ] ),
            x,
            y,
            scale,
            glm::vec3( 1.0f )
        );
    }
}

void View::RenderHand() {
    glUseProgram( _program_sprite_sheet_id );
    glBindVertexArray( _rectangle_gpu.vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, _cards_texture.id );
    glUniform1i( ul( "image" ), 0 );

    for ( int i = 0; i < _hand.size(); ++i )
        RenderCard( _hand[ i ], i );

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
    glm::vec3 translate( card.pos, card.hovered ? -0.1f : index / -1000.0f );

    glm::mat4 world = glm::translate( translate ) * glm::scale( scale );
    glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

    glUniform1f( ul( "stride_x" ), stride_x );
    glUniform1f( ul( "stride_y" ), stride_y );
    glUniform1i( ul( "index_x" ), index_x );
    glUniform1i( ul( "index_y" ), index_y );

    glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );
}

int view::View::CalculateHoveredCardByMousePos( float mouse_x, float mouse_y ) {
    static const float card_ratio = (float)CARD_TEXTURE_WIDTH / CARD_TEXTURE_HEIGHT;
    float card_width = CARD_BASE_SCALE.x * card_ratio / _width * _height;

    if ( mouse_y > _hand_top_y ||
         mouse_x < _hand_start_x - card_width ||
         mouse_x >= _hand_end_x + card_width )
        return -1;

    float interval_length = (_hand_end_x - _hand_start_x) / (_hand.size() - 1);
    float at = (mouse_x - _hand_start_x + interval_length / 2.0f) / interval_length;
    return glm::clamp<int>( static_cast<int>( at ), 0, (int)_hand.size() - 1 );
}

std::tuple<float, float, glm::vec3> View::CalculateParameterPosition( int parameter, int type ) {
    static const float temperature_ratio = (float)_temperature_texture.height / _temperature_texture.width;
    static const float ocean_ratio = (float)_ocean_texture.height / _ocean_texture.width;
    static const float oxygen_ratio = (float)_oxygen_texture.height / _oxygen_texture.width;
    static const float tr_ratio = (float)_tr_texture.height / _tr_texture.width;
    static const float size = 0.06f;
    static const float spacing = size * 2.5f;
    static const float padding = size * 0.5f;

    float x = 1.0f - size - type * spacing / 2.0f;
    float size_x = size / _width * _height;

    float temperature_y = size * temperature_ratio;
    if ( parameter == 0 ) return {
        x,
        1.0f - temperature_y - padding,
        glm::vec3( size_x, size * temperature_ratio, 1.0f )
    };

    float ocean_y = size * ocean_ratio;
    if ( parameter == 1 ) return {
        x,
        1.0f - temperature_y * 2.0f - ocean_y - padding * 2.0f,
        glm::vec3( size_x, size * ocean_ratio, 1.0f )
    };

    float oxygen_y = size * oxygen_ratio;
    if ( parameter == 2 ) return {
        x,
        1.0f - temperature_y * 2.0f - ocean_y * 2.0f - oxygen_y - padding * 3.0f,
        glm::vec3( size_x, size * oxygen_ratio, 1.0f )
    };

    float tr_y = size * tr_ratio;
    if ( parameter == 3 ) return {
        x,
        1.0f - temperature_y * 2.0f - ocean_y * 2.0f - oxygen_y * 2.0f - tr_y - padding * 4.0f,
        glm::vec3( size_x, size * tr_ratio, 1.0f )
    };

    throw std::logic_error( "View::CalculateParameterPosition: received invalid parameter!" );
}

std::tuple<float, float, glm::vec3> View::CalculateResourcePosition( int resource, int type ) {
    static const float size = 0.055f;
    static const float spacing = size * 2.5f;
    return {
        1.0f - size - type * spacing / 2.0f,
        resource * spacing + size * 2.0f - 1.0f,
        glm::vec3( size / _width * _height , size, 1.0f )
    };
}

#pragma endregion Rendering

#pragma region Init and Clean

void View::InitShaders() {
    _program_id = glCreateProgram();
    AttachShader( _program_id, GL_VERTEX_SHADER, "shaders/pos_norm_tex.vert" );
    AttachShader( _program_id, GL_FRAGMENT_SHADER, "shaders/lighting.frag" );
    LinkProgram( _program_id );

    _program_rectangle_id = glCreateProgram();
    AttachShader( _program_rectangle_id, GL_VERTEX_SHADER, "shaders/rectangle.vert" );
    AttachShader( _program_rectangle_id, GL_FRAGMENT_SHADER, "shaders/rectangle.frag" );
    LinkProgram( _program_rectangle_id );

    _program_sprite_sheet_id = glCreateProgram();
    AttachShader( _program_sprite_sheet_id, GL_VERTEX_SHADER, "shaders/sprite_sheet.vert" );
    AttachShader( _program_sprite_sheet_id, GL_FRAGMENT_SHADER, "shaders/sprite_sheet.frag" );
    LinkProgram( _program_sprite_sheet_id );
}

void View::CleanShaders() {
    glDeleteProgram( _program_id );
    glDeleteProgram( _program_rectangle_id );
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


    _cards_texture = LoadTexture( "assets/cards.png" );
    _resources_texture = LoadTexture( "assets/resources.png" );
    _card_cover_texture = LoadTexture( "assets/card_cover.png" );
    _temperature_texture = LoadTexture( "assets/temperature.png" );
    _ocean_texture = LoadTexture( "assets/ocean.png" );
    _oxygen_texture = LoadTexture( "assets/oxygen.png" );
    _tr_texture = LoadTexture( "assets/tr.png" );
    _button_texture = LoadTexture( "assets/button.png" );
    _production_box_texture = LoadTexture( "assets/production_box.png" );


    glBindTexture( GL_TEXTURE_2D, 0 );
}

void View::CleanTextures() {
    glDeleteTextures( 1, &_orange_texture_id );
    glDeleteTextures( 1, &_cards_texture.id );
    glDeleteTextures( 1, &_resources_texture.id );
    glDeleteTextures( 1, &_card_cover_texture.id );
    glDeleteTextures( 1, &_temperature_texture.id );
    glDeleteTextures( 1, &_ocean_texture.id );
    glDeleteTextures( 1, &_oxygen_texture.id );
    glDeleteTextures( 1, &_tr_texture.id );
    glDeleteTextures( 1, &_production_box_texture.id );
}

Texture View::LoadTexture( const std::filesystem::path& filename, GLint wrap_behaviour ) {
    ImageRGBA cards = ImageFromFile( filename );
    Texture tex = { 0, cards.width, cards.height };

    glGenTextures( 1, &tex.id );
    glBindTexture( GL_TEXTURE_2D, tex.id );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, cards.width, cards.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, cards.data() );
    glGenerateMipmap( GL_TEXTURE_2D );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_behaviour );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_behaviour );

    return tex;
}

#pragma endregion Init and Clean

#pragma region Constants

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

#pragma endregion Constants
}
