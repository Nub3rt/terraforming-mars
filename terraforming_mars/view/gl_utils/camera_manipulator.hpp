#pragma once

#include <glm/glm.hpp>

#include <SDL3/SDL_events.h>

#include "camera.hpp"

namespace view
{
class CameraManipulator
{
public:
    virtual ~CameraManipulator() {}

    virtual void Update( float delta ) {}

    virtual void KeyboardDown( const SDL_KeyboardEvent& key ) {}
    virtual void KeyboardUp( const SDL_KeyboardEvent& key ) {}
    virtual void MouseMotion( const SDL_MouseMotionEvent& mouse ) {}
    virtual void MouseDown( const SDL_MouseButtonEvent& mouse ) {}
    virtual void MouseUp( const SDL_MouseButtonEvent& mouse ) {}
    virtual void MouseWheel( const SDL_MouseWheelEvent& wheel ) {}
    virtual void Resize( int w, int h ) {}

    virtual void OtherEvent( const SDL_Event& event ) {}

protected:
    CameraManipulator( Camera& camera ) : _camera( camera ) {}

    Camera& _camera;
};
}
