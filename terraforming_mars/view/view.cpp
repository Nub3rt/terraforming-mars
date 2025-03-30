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
    RenderGlobalParameters();
    RenderResources();
    RenderHand();
}

void View::RenderGlobalParameters() {
    glUseProgram( _program_rectangle_id );
    glBindVertexArray( _rectangle_gpu.vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glUniform1i( ul( "image" ), 0 );


    std::array<GLuint, 4> to_draw = {
        _temperature_texture_id,
        _ocean_texture_id,
        _oxygen_texture_id,
        _tr_texture_id,
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

std::tuple<float, float, glm::vec3> View::CalculateParameterPosition( int parameter, int type ) {
    static const float temperature_ratio = (float)TEMPERATURE_TEXTURE_HEIGHT / TEMPERATURE_TEXTURE_WIDTH;
    static const float ocean_ratio = (float)OCEAN_TEXTURE_HEIGHT / OCEAN_TEXTURE_WIDTH;
    static const float tr_ratio = (float)TR_TEXTURE_HEIGHT / TR_TEXTURE_WIDTH;
    static const float oxygen_ratio = 1.0f;
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

void View::RenderResources() {
    glUseProgram( _program_rectangle_id );
    glBindVertexArray( _rectangle_gpu.vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glUniform1i( ul( "image" ), 0 );

    glBindTexture( GL_TEXTURE_2D, _production_box_texture_id );
    for ( int i = 0; i < +model::Resource::MAX; ++i ) {
        auto [x, y, scale] = CalculateResourcePosition( i, 0 );

        glm::mat4 world = glm::translate( glm::vec3( x, y, 0.0f ) ) * glm::scale( scale );
        glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

        glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );
    }


    glUseProgram( _program_sprite_sheet_id );

    glUniform1i( ul( "image" ), 0 );

    glBindTexture( GL_TEXTURE_2D, _resources_texture_id );
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

std::tuple<float, float, glm::vec3> View::CalculateResourcePosition( int resource, int type ) {
    static const float size = 0.06f;
    static const float spacing = size * 2.5f;
    return {
        1.0f - size - type * spacing / 2.0f,
        resource * spacing + size * 2.0f - 1.0f,
        glm::vec3( size / _width * _height , size, 1.0f )
    };
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


    LoadTexture( &_cards_texture_id, "assets/cards.png" );
    LoadTexture( &_resources_texture_id, "assets/resources.png" );
    LoadTexture( &_card_cover_texture_id, "assets/card_cover.png" );
    LoadTexture( &_temperature_texture_id, "assets/temperature.png" );
    LoadTexture( &_ocean_texture_id, "assets/ocean.png" );
    LoadTexture( &_oxygen_texture_id, "assets/oxygen.png" );
    LoadTexture( &_tr_texture_id, "assets/tr.png" );
    LoadTexture( &_production_box_texture_id, "assets/production_box.png" );


    glBindTexture( GL_TEXTURE_2D, 0 );
}

void View::CleanTextures() {
    glDeleteTextures( 1, &_orange_texture_id );
    glDeleteTextures( 1, &_cards_texture_id );
    glDeleteTextures( 1, &_resources_texture_id );
    glDeleteTextures( 1, &_card_cover_texture_id );
    glDeleteTextures( 1, &_temperature_texture_id );
    glDeleteTextures( 1, &_ocean_texture_id );
    glDeleteTextures( 1, &_oxygen_texture_id );
    glDeleteTextures( 1, &_tr_texture_id );
    glDeleteTextures( 1, &_production_box_texture_id );
}

void View::LoadTexture( GLuint* id, const std::filesystem::path& filename, GLint wrap_behaviour ) {
    ImageRGBA cards = ImageFromFile( filename );

    glGenTextures( 1, id );
    glBindTexture( GL_TEXTURE_2D, *id );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, cards.width, cards.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, cards.data() );
    glGenerateMipmap( GL_TEXTURE_2D );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_behaviour );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_behaviour );
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
