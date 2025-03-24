#include "app.h"

#include <imgui.h>

#include "gl_utils/gl_utils.h"
#include "gl_utils/SDL_GLDebugMessageCallback.h"

namespace view
{
App::App() : _camera() {
}

App::~App() {
}

bool App::Init() {
    SetupDebugCallback();

    glClearColor( 0.125f, 0.25f, 0.5f, 1.0f );

    InitSkyboxShaders();
    InitSkyboxGeometry();
    InitSkyboxTextures();

    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );

    glEnable( GL_DEPTH_TEST );

    _camera.SetView(
        glm::vec3( 0.0f, 40.0f, 0.0f ),
        glm::vec3( 0.0f, 0.0f, 0.0f ),
        glm::vec3( 0.0f, 1.0f, 0.0f )
    );

    _camera_manipulator = new SphericalCameraManipulator();
    _camera_manipulator->SetCamera( &_camera );

    return true;
}

void App::Clean() {
    CleanSkyboxShaders();
    CleanSkyboxGeometry();
    CleanSkyboxTextures();

    delete _camera_manipulator;
}

void App::Update( const UpdateInfo& update_info ) {
    _camera_manipulator->Update( update_info.delta );
}

void App::Render() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    RenderSkybox();
}

void App::RenderGUI() {
}

void App::KeyboardDown( const SDL_KeyboardEvent& key ) {
    _camera_manipulator->KeyboardDown( key );
}

void App::KeyboardUp( const SDL_KeyboardEvent& key ) {
    _camera_manipulator->KeyboardUp( key );
}

void App::MouseMotion( const SDL_MouseMotionEvent& mouse ) {
    _camera_manipulator->MouseMove( mouse );
}

void App::MouseDown( const SDL_MouseButtonEvent& mouse ) {
}

void App::MouseUp( const SDL_MouseButtonEvent& mouse ) {
}

void App::MouseWheel( const SDL_MouseWheelEvent& wheel ) {
    _camera_manipulator->MouseWheel( wheel );
}

void App::Resize( int w, int h ) {
    glViewport( 0, 0, w, h );
    _camera.SetAspect( w / (float)h );
}

void App::OtherEvent( const SDL_Event& event ) {
}

void App::SetupDebugCallback() {
    GLint context_flags;
    glGetIntegerv( GL_CONTEXT_FLAGS, &context_flags );
    if ( context_flags & GL_CONTEXT_FLAG_DEBUG_BIT ) {
        glEnable( GL_DEBUG_OUTPUT );
        glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
        glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE );
        glDebugMessageCallback( SDL_GLDebugMessageCallback, nullptr );
    }
}

void App::RenderSkybox() {
    glUseProgram( _program_skybox_id );

    glBindVertexArray( _skybox_gpu.vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_CUBE_MAP, _skybox_texture_id );

    glUniformMatrix4fv( ul( "view_proj" ), 1, GL_FALSE, glm::value_ptr( _camera.GetViewProj() ) );
    glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( glm::translate( _camera.GetEye() ) ) );
    glUniform1i( ul( "skybox_texture" ), 0 );

    GLint prev_depth_func;
    glGetIntegerv( GL_DEPTH_FUNC, &prev_depth_func );

    glDepthFunc( GL_LEQUAL );

    glDrawElements( GL_TRIANGLES, _skybox_gpu.count, GL_UNSIGNED_INT, nullptr );

    glDepthFunc( prev_depth_func );

    glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
    glUseProgram( 0 );
}

void App::InitSkyboxGeometry() {
    MeshObject<glm::vec3> skybox_cpu = {
        std::vector<glm::vec3> {
            glm::vec3( -1, -1, -1 ),
            glm::vec3(  1, -1, -1 ),
            glm::vec3(  1,  1, -1 ),
            glm::vec3( -1,  1, -1 ),

            glm::vec3( -1, -1,  1 ),
            glm::vec3(  1, -1,  1 ),
            glm::vec3(  1,  1,  1 ),
            glm::vec3( -1,  1,  1 ),
        },
        std::vector<GLuint>
        {
            // front
            0, 1, 2,
            2, 3, 0,
            // back
            4, 6, 5,
            6, 4, 7,
            // left
            0, 3, 4,
            4, 3, 7,
            // right
            1, 5, 2,
            5, 6, 2,
            // bottom
            1, 0, 4,
            1, 4, 5,
            // top
            3, 2, 6,
            3, 6, 7,
        }
    };

    _skybox_gpu = CreateGLObjectFromMesh( skybox_cpu, { { 0, offsetof( glm::vec3, x ), 3, GL_FLOAT } } );
}

void App::CleanSkyboxGeometry() {
    CleanOGLObject( _skybox_gpu );
}

void App::InitSkyboxShaders() {
    _program_skybox_id = glCreateProgram();
    AttachShader( _program_skybox_id, GL_VERTEX_SHADER, "shaders/skybox.vert" );
    AttachShader( _program_skybox_id, GL_FRAGMENT_SHADER, "shaders/skybox.frag" );
    LinkProgram( _program_skybox_id );
}

void App::CleanSkyboxShaders() {
    glDeleteProgram( _program_skybox_id );
}

void App::InitSkyboxTextures() {
    ImageRGBA xpos = ImageFromFile( "assets/right.png", false );
    ImageRGBA xneg = ImageFromFile( "assets/left.png", false );
    ImageRGBA ypos = ImageFromFile( "assets/top.png", false );
    ImageRGBA yneg = ImageFromFile( "assets/bottom.png", false );
    ImageRGBA zpos = ImageFromFile( "assets/front.png", false );
    ImageRGBA zneg = ImageFromFile( "assets/back.png", false );

    glCreateTextures( GL_TEXTURE_CUBE_MAP, 1, &_skybox_texture_id );
    glBindTexture( GL_TEXTURE_CUBE_MAP, _skybox_texture_id );
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, xpos.width, xpos.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, xpos.data() );
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, xneg.width, xneg.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, xneg.data() );
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, ypos.width, ypos.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ypos.data() );
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, yneg.width, yneg.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, yneg.data() );
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, zpos.width, zpos.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, zpos.data() );
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, zneg.width, zneg.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, zneg.data() );

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

    glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );

    glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
}

void App::CleanSkyboxTextures() {
    glDeleteTextures( 1, &_skybox_texture_id );
}
}
