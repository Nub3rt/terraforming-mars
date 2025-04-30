#include "tex_store.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "gl_utils/gl_utils.hpp"

namespace view
{

bool TexStore::Init() {
    InitShaders();
    InitGeometry();
    InitTextures();

    return true;
}

void TexStore::Clean() {
    CleanShaders();
    CleanGeometry();
    CleanTextures();
}

void TexStore::InitShaders() {
    program_id = glCreateProgram();
    AttachShader( program_id, GL_VERTEX_SHADER, "shaders/pos_norm_tex.vert" );
    AttachShader( program_id, GL_FRAGMENT_SHADER, "shaders/lighting.frag" );
    LinkProgram( program_id );

    program_card_id = glCreateProgram();
    AttachShader( program_card_id, GL_VERTEX_SHADER, "shaders/sprite_sheet.vert" );
    AttachShader( program_card_id, GL_FRAGMENT_SHADER, "shaders/card.frag" );
    LinkProgram( program_card_id );

    program_rectangle_id = glCreateProgram();
    AttachShader( program_rectangle_id, GL_VERTEX_SHADER, "shaders/rectangle.vert" );
    AttachShader( program_rectangle_id, GL_FRAGMENT_SHADER, "shaders/rectangle.frag" );
    LinkProgram( program_rectangle_id );

    program_sprite_sheet_id = glCreateProgram();
    AttachShader( program_sprite_sheet_id, GL_VERTEX_SHADER, "shaders/sprite_sheet.vert" );
    AttachShader( program_sprite_sheet_id, GL_FRAGMENT_SHADER, "shaders/sprite_sheet.frag" );
    LinkProgram( program_sprite_sheet_id );
}

void TexStore::CleanShaders() {
    glDeleteProgram( program_id );
    glDeleteProgram( program_card_id );
    glDeleteProgram( program_rectangle_id );
    glDeleteProgram( program_sprite_sheet_id );
}

void TexStore::InitGeometry() {
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

    hexagon_gpu = CreateGLObjectFromMesh( hexagon_cpu, vertex_plus_attribute_list );


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

    rectangle_gpu = CreateGLObjectFromMesh( rectangle_cpu, vertex_pos_tex_attribute_list );
}

void TexStore::CleanGeometry() {
    CleanOGLObject( hexagon_gpu );
    CleanOGLObject( rectangle_gpu );
}

void TexStore::InitTextures() {
    cards_texture = LoadTexture( "assets/cards.png" );
    resources_texture = LoadTexture( "assets/resources.png" );

    temperature_texture = LoadTexture( "assets/temperature.png" );
    oxygen_texture = LoadTexture( "assets/oxygen.png" );
    tr_texture = LoadTexture( "assets/tr.png" );

    ocean_texture = LoadTexture( "assets/ocean.png" );
    greenery_texture = LoadTexture( "assets/greenery.png" );
    city_texture = LoadTexture( "assets/city.png" );

    button_short_texture = LoadTexture( "assets/button_short.png" );
    button_long_texture = LoadTexture( "assets/button_long.png" );
    production_box_texture = LoadTexture( "assets/production_box.png" );
    arrow_texture = LoadTexture( "assets/arrow.png" );
    player_icon_texture = LoadTexture( "assets/player.png" );
    card_cover_texture = LoadTexture( "assets/card_cover.png" );

    action_closed_texture = LoadTexture( "assets/action_closed.png" );
    action_open_texture = LoadTexture( "assets/action_open.png" );
    event_closed_texture = LoadTexture( "assets/event_closed.png" );
    event_open_texture = LoadTexture( "assets/event_open.png" );
    automated_closed_texture = LoadTexture( "assets/automated_closed.png" );
    automated_open_texture = LoadTexture( "assets/automated_open.png" );
    effect_closed_texture = LoadTexture( "assets/effect_closed.png" );
    effect_open_texture = LoadTexture( "assets/effect_open.png" );
}

void TexStore::CleanTextures() {
    glDeleteTextures( 1, &cards_texture.id );
    glDeleteTextures( 1, &resources_texture.id );

    glDeleteTextures( 1, &temperature_texture.id );
    glDeleteTextures( 1, &oxygen_texture.id );
    glDeleteTextures( 1, &tr_texture.id );

    glDeleteTextures( 1, &ocean_texture.id );
    glDeleteTextures( 1, &greenery_texture.id );
    glDeleteTextures( 1, &city_texture.id );

    glDeleteTextures( 1, &button_short_texture.id );
    glDeleteTextures( 1, &button_long_texture.id );
    glDeleteTextures( 1, &production_box_texture.id );
    glDeleteTextures( 1, &arrow_texture.id );
    glDeleteTextures( 1, &player_icon_texture.id );
    glDeleteTextures( 1, &card_cover_texture.id );

    glDeleteTextures( 1, &action_closed_texture.id );
    glDeleteTextures( 1, &action_open_texture.id );
    glDeleteTextures( 1, &event_closed_texture.id );
    glDeleteTextures( 1, &event_open_texture.id );
    glDeleteTextures( 1, &automated_closed_texture.id );
    glDeleteTextures( 1, &automated_open_texture.id );
    glDeleteTextures( 1, &effect_closed_texture.id );
    glDeleteTextures( 1, &effect_open_texture.id );
}
}
