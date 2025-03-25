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

namespace view
{
class View
{
public:
    View();
    ~View();

    bool Init( Camera* camera );
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
    Camera* _camera = nullptr;


    GLuint _program_id = 0;
    GLuint _program_ui_id = 0;

    void InitShaders();
    void CleanShaders();

    OGLObject _hexagon_gpu = {};

    void InitGeometry();
    void CleanGeometry();

    GLuint _orange_texture_id = 0;

    void InitTextures();
    void CleanTextures();

    static const std::initializer_list<VertexAttributeDescriptor> _vertex_attribute_list;
};
}
