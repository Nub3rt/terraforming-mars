#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

namespace view
{
Camera::Camera() {
    SetView( glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 0.0f, -1.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

Camera::~Camera() {}

void Camera::SetView( glm::vec3 eye, glm::vec3 at, glm::vec3 world_up ) {
    _eye = eye;
    _at = at;
    _world_up = world_up;

    _view_matrix = glm::lookAt( _eye, _at, _world_up );
}

void Camera::SetProj( float angle, float aspect, float z_near, float z_far ) {
    _angle = angle;
    _aspect = aspect;
    _z_near = z_near;
    _z_far = z_far;

    _proj_matrix = glm::perspective( _angle, _aspect, _z_near, _z_far );
}

void Camera::SetAngle( float angle ) noexcept {
    _angle = angle;
    _proj_matrix = glm::perspective( _angle, _aspect, _z_near, _z_far );
}

void Camera::SetAspect( float aspect ) noexcept {
    _aspect = aspect;
    _proj_matrix = glm::perspective( _angle, _aspect, _z_near, _z_far );
}

void Camera::SetZNear( float zn ) noexcept {
    _z_near = zn;
    _proj_matrix = glm::perspective( _angle, _aspect, _z_near, _z_far );
}

void Camera::SetZFar( float zf ) noexcept {
    _z_far = zf;
    _proj_matrix = glm::perspective( _angle, _aspect, _z_near, _z_far );
}
}
