#pragma once

#include <glm/glm.hpp>

#include <SDL3/SDL_events.h>

#include "camera.hpp"
#include "camera_manipulator.hpp"

namespace view
{
class TMCameraManipulator : public CameraManipulator
{
public:
    TMCameraManipulator( Camera& camera );

    ~TMCameraManipulator();

    void Update( float delta ) override;

    void KeyboardDown( const SDL_KeyboardEvent& key ) override;
    void MouseMotion( const SDL_MouseMotionEvent& mouse ) override;
    void MouseWheel( const SDL_MouseWheelEvent& wheel ) override;
    void Resize( int w, int h ) override;

protected:
    // The x position of the mouse
    float _x = 0.0f;

    // The y position of the mouse
    float _y = 0.0f;

    // The width of the screen
    int _w = 0;

    // The height of the screen
    int _h = 0;

    // The distance of the look at point from the camera
    float _distance = 0.0f;

    // The default distance
    float _default_distance = 0.0f;

    // The view point, provided (_x, _y) == (0, 0)
    glm::vec3 _eye = glm::vec3( 0.0f, 0.0f, 1.0f );

    // The center of model sphere
    glm::vec3 _center = glm::vec3( 0.0f );

    static constexpr float max_sideways_bob = 5.0f * 2.0f;
};
}
