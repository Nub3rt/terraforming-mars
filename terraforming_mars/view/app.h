#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <GL/glew.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "gl_utils/camera.h"
#include "gl_utils/spherical_camera_manipulator.h"
#include "gl_utils/gl_utils.h"

struct UpdateInfo
{
    float elapsed = 0.0f;
    float delta = 0.0f;
};

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

protected:
    Camera _camera;
    SphericalCameraManipulator* _camera_manipulator = nullptr;

    void SetupDebugCallback();

    void RenderSkybox();

    OGLObject _skybox_gpu = {};
    void InitSkyboxGeometry();
    void CleanSkyboxGeometry();

    GLuint _program_skybox_id = 0;
    void InitSkyboxShaders();
    void CleanSkyboxShaders();

    GLuint _skybox_texture_id = 0;
    void InitSkyboxTextures();
    void CleanSkyboxTextures();
};
}
