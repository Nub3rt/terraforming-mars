#pragma once

#include <glm/glm.hpp>

#include <SDL3/SDL_events.h>

#include "camera.h"

namespace view
{
class SphericalCameraManipulator
{
public:
    SphericalCameraManipulator();

    ~SphericalCameraManipulator();

    void SetCamera( Camera* camera );
    void Update( float delta );

    inline void SetSpeed( float speed ) { _speed = speed; }
    inline float GetSpeed() const noexcept { return _speed; }

    void KeyboardDown( const SDL_KeyboardEvent& key );
    void KeyboardUp( const SDL_KeyboardEvent& key );
    void MouseMove( const SDL_MouseMotionEvent& mouse );
    void MouseWheel( const SDL_MouseWheelEvent& wheel );

private:
    Camera* _camera = nullptr;

    // The u spherical coordinate of the spherical coordinate pair (u,v) denoting the
    // current viewing direction from the view position _eye
    float _u = 0.0f;

    // The v spherical coordinate of the spherical coordinate pair (u,v) denoting the
    // current viewing direction from the view position _eye
    float _v = 0.0f;

    // The distance of the look at point from the camera
    float _distance = 0.0f;

    // The center of model sphere
    glm::vec3 _center = glm::vec3( 0.0f );

    // The world-up vector of the camera
    glm::vec3 _world_up = glm::vec3( 0.0f, 1.0f, 0.0f );

    // The traversal speed of the camera
    float _speed = 16.0f;

    // Traveling indicator to different directions
    float _go_forward = 0.0f;
    float _go_right = 0.0f;
    float _go_up = 0.0f;
};
}
