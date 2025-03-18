#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/transform.hpp>

#include <GL/glew.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

//#include "Camera.h"
//#include "CameraManipulator.h"
//#include "GLUtils.hpp"

struct UpdateInfo
{
    float elapsed = 0.0f;
    float delta = 0.0f;
};

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
    void SetupDebugCallback();

    float green, blue, red = 0;
    int x_resolution, y_resolution;
};
