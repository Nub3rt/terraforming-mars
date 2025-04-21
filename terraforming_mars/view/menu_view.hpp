#pragma once

#include <functional>

#include <glm/glm.hpp>
#include <glm/gtx/easing.hpp>

#include <GL/glew.h>

#include "animatable.hpp"
#include "builder.hpp"
#include "constants.hpp"
#include "view.hpp"
#include "game_view.hpp"

#include "gl_utils/camera.hpp"
#include "gl_utils/gl_utils.hpp"
#include "gl_utils/spherical_camera_manipulator.hpp"

#include "../model/event.hpp"

namespace view
{
class MenuView : public View
{
public:
    MenuView();
    virtual ~MenuView();

    bool Init( Camera* camera );
    void Clean() override;

    void Update( const UpdateInfo& update_info ) override;
    void Render() override;
    void RenderGUI() override;

    void KeyboardDown( const SDL_KeyboardEvent& key ) override;
    void MouseMotion( const SDL_MouseMotionEvent& mouse ) override;
    void MouseDown( const SDL_MouseButtonEvent& mouse ) override;
    void MouseUp( const SDL_MouseButtonEvent& mouse ) override;
    void Resize( int w, int h ) override;

    void TransitionFromMars();

    inline void SetNewGame( std::function<void( GameView* )> callback ) { _new_game.SetCallback( callback ); }
    inline void SetEnterGame( std::function<void()> callback ) { _enter_game.SetCallback( callback ); }
    inline void SetQuit( std::function<void()> callback ) { _quit.SetCallback( callback ); }

protected:
    int _width = 0;
    int _height = 0;

    Camera* _camera = nullptr;

    Builder _builder;
    GameView* _game_view = nullptr;

    bool _can_continue = false;
    bool _transitioning = false;
    bool _to_mars = true;
    uint8_t _mouse_hover_stencil = 0;
    uint8_t _mouse_down_stencil = 0;

    Animatable<glm::vec3> _eye = glm::vec3( CAMERA_X, CAMERA_Y, CAMERA_Z_MENU_EYE );
    Animatable<glm::vec3> _at = glm::vec3( CAMERA_X, CAMERA_Y, CAMERA_Z_MENU_AT );

    model::Event<GameView*> _new_game;
    model::Event<> _enter_game;
    model::Event<> _quit;


    void TransitionToMars();
    void Transition( float eye_from, float eye_to, float at_from, float at_to );

    uint8_t GetStencilValue( float mouse_x, float mouse_y );

    inline void SetStencilRef( GLint ref = STENCIL_NONE ) { glStencilFunc( GL_ALWAYS, ref, 0xff ); }


    GLuint _program_pane_id = 0;

    void InitShaders();
    void CleanShaders();

    OGLObject _rectangle_gpu = {};

    void InitGeometry();
    void CleanGeometry();

    Texture _tm_texture = {};
    Texture _continue_texture = {};
    Texture _new_game_texture = {};
    Texture _quit_texture = {};

    void InitTextures();
    void CleanTextures();


    bool _debug = false;
    int _seed = 42;
};
}
