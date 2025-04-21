#include "tm_camera_manipulator.hpp"

#include <glm/glm.hpp>

#include <SDL3/SDL.h>

#include "camera.hpp"
#include "camera_manipulator.hpp"

#include "../constants.hpp"

namespace view
{
TMCameraManipulator::TMCameraManipulator( Camera& camera )
        : CameraManipulator( camera ) {
    _center = glm::vec3( CAMERA_X, CAMERA_Y, CAMERA_Z_GAME_AT );

    _eye = glm::vec3( CAMERA_X, CAMERA_Y, CAMERA_Z_GAME_EYE );

    _distance = glm::length( _center - _eye );
    _default_distance = _distance;

    _eye = glm::normalize( _eye );
}

TMCameraManipulator::~TMCameraManipulator() {}

void TMCameraManipulator::Update( float delta ) {
    glm::vec3 eye = _center + _distance * _eye;
    eye.x += _x * max_sideways_bob;
    eye.y += _y * max_sideways_bob;

    glm::vec3 corrected_dir( eye - _center );
    corrected_dir = corrected_dir * (_distance / glm::length( corrected_dir ));
    eye = _center + corrected_dir;

    glm::vec3 up = _camera.GetWorldUp();

    _camera.SetView( eye, _center, up );
}

void TMCameraManipulator::KeyboardDown( const SDL_KeyboardEvent& key ) {
    if ( key.key == SDLK_SPACE )
        _distance = _default_distance;
}

void TMCameraManipulator::MouseMotion( const SDL_MouseMotionEvent& mouse ) {
    _x =       mouse.x  / _w * 2.0f - 1.0f;
    _y = (_h - mouse.y) / _h * 2.0f - 1.0f;
}

void TMCameraManipulator::MouseWheel( const SDL_MouseWheelEvent& wheel ) {
    _distance *= powf( 0.9f, wheel.y );
}
void TMCameraManipulator::Resize( int w, int h ) {
    _w = w;
    _h = h;
}
}
