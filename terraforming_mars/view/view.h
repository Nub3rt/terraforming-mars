#pragma once

#include <array>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <GL/glew.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "card_wrapper.h"
#include "constants.h"
#include "tile_wrapper.h"
#include "gl_utils/camera.h"
#include "gl_utils/spherical_camera_manipulator.h"
#include "gl_utils/gl_utils.h"

#include "../model/game_model.h"
#include "../model/resource.h"

namespace view
{
class View
{
public:
    View();
    ~View();

    bool Init( Camera* camera, model::GameModel* model );
    void Clean();

    void Update( const UpdateInfo& update_info );
    void Render();
    void RenderGUI();

    void KeyboardDown( const SDL_KeyboardEvent& key );
    void KeyboardUp( const SDL_KeyboardEvent& key );
    void MouseMotion( const SDL_MouseMotionEvent& mouse );
    void MouseDown( const SDL_MouseButtonEvent& mouse );
    void MouseUp( const SDL_MouseButtonEvent& mouse );
    void MouseWheel( const SDL_MouseWheelEvent& wheel );
    void Resize( int w, int h );

    void OtherEvent( const SDL_Event& event );

protected:
    int _width = 0;
    int _height = 0;

    float _elapsed = 0.0f;

    Camera* _camera = nullptr;
    SphericalCameraManipulator* _camera_manipulator = nullptr;


    model::GameModel* _model = nullptr;

    int _generation = 0;
    int _temperature = 0;
    int _ocean_count = 0;
    int _oxygen_level = 0;
    int _tr = 0;
    std::array<int, +model::Resource::MAX + 1> _resources;
    std::array<int, +model::Resource::MAX + 1> _resource_productions;
    std::vector<TileWrapper> _tiles;
    int _starting_card_id = 0;


    void RenderBoard();
    void RenderHexagon( TileWrapper& tile, int id );

    void RenderHUD();
    void RenderGlobalParameters();
    std::tuple<float, float, glm::vec3> CalculateParameterPosition( int parameter, int type );
    void RenderResources();
    std::tuple<float, float, glm::vec3> CalculateResourcePosition( int resource, int type );
    void RenderHand();
    void RenderCard( CardWrapper& card, int index );

    inline void SetStencilRef( GLint ref = STENCIL_NONE ) { glStencilFunc( GL_ALWAYS, ref, 0xff ); }


    GLuint _program_id = 0;
    GLuint _program_rectangle_id = 0;
    GLuint _program_sprite_sheet_id = 0;

    void InitShaders();
    void CleanShaders();

    OGLObject _hexagon_gpu = {};
    OGLObject _rectangle_gpu = {};

    void InitGeometry();
    void CleanGeometry();

    GLuint _orange_texture_id = 0;
    GLuint _cards_texture_id = 0;
    GLuint _resources_texture_id = 0;
    GLuint _card_cover_texture_id = 0;
    GLuint _temperature_texture_id = 0;
    GLuint _ocean_texture_id = 0;
    GLuint _oxygen_texture_id = 0;
    GLuint _tr_texture_id = 0;
    GLuint _production_box_texture_id = 0;

    void InitTextures();
    void CleanTextures();
    void LoadTexture( GLuint* id, const std::filesystem::path& filename, GLint wrap_behaviour = GL_CLAMP_TO_EDGE );

    virtual const std::pair<float, float>& GetBoardOrigin();

    static const std::initializer_list<VertexAttributeDescriptor> _vertex_pos_tex_attribute_list;
    static const std::initializer_list<VertexAttributeDescriptor> _vertex_attribute_list;
    static const std::initializer_list<VertexAttributeDescriptor> _vertex_plus_attribute_list;
};
}
