#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <GL/glew.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "game_view.hpp"
#include "menu_view.hpp"
#include "view.hpp"

#include "gl_utils/camera.hpp"
#include "gl_utils/spherical_camera_manipulator.hpp"
#include "gl_utils/gl_utils.hpp"

namespace view
{
class App
{
public:
    App();
    ~App();

    bool Init();
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

    inline void SetQuit( std::function<void()> callback ) { _quit.SetCallback( callback ); }

protected:
    float _elapsed = 0.0f;

    Camera _camera;

    View* _current_view = nullptr;
    MenuView* _menu_view = nullptr;
    GameView* _game_view = nullptr;

    model::Event<> _quit;

    void SetupDebugCallback();

    void RenderSkybox();


    GLuint _program_skybox_id = 0;

    void InitSkyboxShaders();
    void CleanSkyboxShaders();

    OGLObject _skybox_gpu = {};

    void InitSkyboxGeometry();
    void CleanSkyboxGeometry();

    GLuint _skybox_texture_id = 0;

    void InitSkyboxTextures();
    void CleanSkyboxTextures();
};
}
