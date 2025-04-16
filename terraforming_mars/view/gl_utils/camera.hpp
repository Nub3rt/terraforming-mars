#pragma once

#include <glm/glm.hpp>

namespace view
{
class Camera
{
public:
    Camera();
    ~Camera();

    inline glm::vec3 GetEye() const { return _eye; }
    inline glm::vec3 GetAt() const { return _at; }
    inline glm::vec3 GetWorldUp() const { return _world_up; }

    inline glm::mat4 GetViewMatrix() const { return _view_matrix; }
    inline glm::mat4 GetProj() const { return _proj_matrix; }
    inline glm::mat4 GetViewProj() const { return _proj_matrix * _view_matrix; }

    void SetView( glm::vec3 eye, glm::vec3 at, glm::vec3 up );

    inline float GetAngle() const { return _angle; }
    void SetAngle( float angle ) noexcept;
    inline float GetAspect() const { return _aspect; }
    void SetAspect( float aspect ) noexcept;
    inline float GetZNear() const { return _z_near; }
    void SetZNear( float zn ) noexcept;
    inline float GetZFar() const { return _z_far; }
    void SetZFar( float zf ) noexcept;

    void SetProj( float angle, float aspect, float z_near, float z_far );

private:

    // camera position
    glm::vec3 _eye;

    // vector pointing upwards
    glm::vec3 _world_up;

    // camera look at point
    glm::vec3 _at;

    // view matrix of the camera
    glm::mat4 _view_matrix;

    // projection parameters
    float _z_near = 0.01f;
    float _z_far = 1000.0f;

    float _angle = glm::radians( 27.0f );
    float _aspect = 1.0f;

    // projection matrix
    glm::mat4 _proj_matrix;
};
}
