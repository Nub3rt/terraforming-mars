#include "spherical_camera_manipulator.hpp"

#include <glm/glm.hpp>

#include <SDL3/SDL.h>

#include "camera.hpp"
#include "camera_manipulator.hpp"

#include "../constants.hpp"

namespace view
{
SphericalCameraManipulator::SphericalCameraManipulator( Camera& camera )
    : CameraManipulator( camera ) {
    _center = glm::vec3( CAMERA_X, CAMERA_Y, CAMERA_Z_GAME_AT );

    glm::vec3 to_aim = _center - glm::vec3( CAMERA_X, CAMERA_Y, CAMERA_Z_GAME_EYE );

    _distance = glm::length( to_aim );

    _u = atan2f( to_aim.z, to_aim.x );
    _v = acosf( to_aim.y / _distance );
}

SphericalCameraManipulator::~SphericalCameraManipulator() {}

void SphericalCameraManipulator::Update( float delta ) {
    glm::vec3 look_direction( cosf( _u ) * sinf( _v ),
                              cosf( _v ),
                              sinf( _u ) * sinf( _v ) );

    glm::vec3 eye = _center - _distance * look_direction;

    glm::vec3 up = _camera.GetWorldUp();

    glm::vec3 right = glm::normalize( glm::cross( look_direction, up ) );

    glm::vec3 forward = glm::cross( up, right );

    glm::vec3 delta_position = (_go_forward * forward + _go_right * right + _go_up * up) * _speed * delta;

    eye += delta_position;
    _center += delta_position;

    _camera.SetView( eye, _center, up );
}

void SphericalCameraManipulator::KeyboardDown( const SDL_KeyboardEvent& key ) {
    switch ( key.key ) {
        case SDLK_LCTRL:
        case SDLK_RCTRL:
            if ( !key.repeat )
                _speed *= 4.0f;
            break;
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            if ( !key.repeat )
                _speed /= 4.0f;
            break;
        case SDLK_W:
            _go_forward = 1;
            break;
        case SDLK_S:
            _go_forward = -1;
            break;
        case SDLK_D:
            _go_right = 1;
            break;
        case SDLK_A:
            _go_right = -1;
            break;
        case SDLK_E:
            _go_up = 1;
            break;
        case SDLK_Q:
            _go_up = -1;
            break;
    }
}

void SphericalCameraManipulator::KeyboardUp( const SDL_KeyboardEvent& key ) {
    switch ( key.key ) {
        case SDLK_LCTRL:
        case SDLK_RCTRL:
            _speed /= 4.0f;
            break;
        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            _speed *= 4.0f;
            break;
        case SDLK_W:
        case SDLK_S:
            _go_forward = 0;
            break;
        case SDLK_A:
        case SDLK_D:
            _go_right = 0;
            break;
        case SDLK_Q:
        case SDLK_E:
            _go_up = 0;
            break;
    }
}

void SphericalCameraManipulator::MouseMotion( const SDL_MouseMotionEvent& mouse ) {
    if ( mouse.state & SDL_BUTTON_LMASK ) {
        float du = mouse.xrel / 100.0f;
        float dv = mouse.yrel / 100.0f;

        _u += du;
        _v = glm::clamp<float>( _v + dv, 0.001f, 3.14f );
    }
    else if ( mouse.state & SDL_BUTTON_RMASK ) {
        _distance *= powf( 0.9f, mouse.yrel / 50.0f );
    }
}

void SphericalCameraManipulator::MouseWheel( const SDL_MouseWheelEvent& wheel ) {
    _distance *= powf( 0.9f, wheel.y );
}
}
