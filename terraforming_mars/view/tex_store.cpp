#include "tex_store.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "gl_utils/gl_utils.hpp"
#include "gl_utils/obj_parser.hpp"

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


    MeshObject<Vertex> tile_bottom_mesh = ObjParser::Parse( "objects/tile_bottom.obj" );
    MeshObject<Vertex> tile_top_mesh = ObjParser::Parse( "objects/tile_empty_top.obj" );
    MeshObject<Vertex> planes_mesh = ObjParser::Parse( "objects/plains.obj" );
    MeshObject<Vertex> planes_greenery_mesh = ObjParser::Parse( "objects/plains_greenery.obj" );
    MeshObject<Vertex> planes_city_mesh = ObjParser::Parse( "objects/plains_city.obj" );
    MeshObject<Vertex> dunes_mesh = ObjParser::Parse( "objects/dunes.obj" );
    MeshObject<Vertex> dunes_greenery_mesh = ObjParser::Parse( "objects/dunes_greenery.obj" );
    MeshObject<Vertex> dunes_city_mesh = ObjParser::Parse( "objects/dunes_city.obj" );
    MeshObject<Vertex> mountains_mesh = ObjParser::Parse( "objects/mountains.obj" );
    MeshObject<Vertex> mountains_greenery_mesh = ObjParser::Parse( "objects/mountains_greenery.obj" );
    MeshObject<Vertex> mountains_city_mesh = ObjParser::Parse( "objects/mountains_city.obj" );

    tile_bottom = CreateGLObjectFromMesh( tile_bottom_mesh, vertex_attribute_list );
    tile_top = CreateGLObjectFromMesh( tile_top_mesh, vertex_attribute_list );
    plains = CreateGLObjectFromMesh( planes_mesh, vertex_attribute_list );
    plains_greenery = CreateGLObjectFromMesh( planes_greenery_mesh, vertex_attribute_list );
    plains_city = CreateGLObjectFromMesh( planes_city_mesh, vertex_attribute_list );
    dunes = CreateGLObjectFromMesh( dunes_mesh, vertex_attribute_list );
    dunes_greenery = CreateGLObjectFromMesh( dunes_greenery_mesh, vertex_attribute_list );
    dunes_city = CreateGLObjectFromMesh( dunes_city_mesh, vertex_attribute_list );
    mountains = CreateGLObjectFromMesh( mountains_mesh, vertex_attribute_list );
    mountains_greenery = CreateGLObjectFromMesh( mountains_greenery_mesh, vertex_attribute_list );
    mountains_city = CreateGLObjectFromMesh( mountains_city_mesh, vertex_attribute_list );
}

void TexStore::CleanGeometry() {
    CleanOGLObject( hexagon_gpu );
    CleanOGLObject( rectangle_gpu );

    CleanOGLObject( tile_bottom );
    CleanOGLObject( tile_top );
    CleanOGLObject( plains );
    CleanOGLObject( plains_greenery );
    CleanOGLObject( plains_city );
    CleanOGLObject( dunes );
    CleanOGLObject( dunes_greenery );
    CleanOGLObject( dunes_city );
    CleanOGLObject( mountains );
    CleanOGLObject( mountains_greenery );
    CleanOGLObject( mountains_city );
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

    terrain_mars_texture = LoadTexture( "assets/terrain_mars.png" );
    terrain_greenery_texture = LoadTexture( "assets/terrain_greenery.jpg" );
    terrain_ocean_texture = LoadTexture( "assets/terrain_ocean.jpg" );
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
