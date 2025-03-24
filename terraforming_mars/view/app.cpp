#include "app.h"

#include <imgui.h>

#include "gl_utils/SDL_GLDebugMessageCallback.h"

App::App() {
}

App::~App() {
}

bool App::Init() {
    SetupDebugCallback();

    glClearColor( 0.125f, 0.25f, 0.5f, 1.0f );

    glEnable( GL_CULL_FACE ); // kapcsoljuk be a hátrafelé néző lapok eldobását
    glCullFace( GL_BACK );    // GL_BACK: a kamerától "elfelé" néző lapok, GL_FRONT: a kamera felé néző lapok

    glEnable( GL_DEPTH_TEST ); // mélységi teszt bekapcsolása (takarás)

    return true;
}

void App::Clean() {
}

void App::Update( const UpdateInfo& update_info ) {
}

void App::Render() {
    glClearColor( red, green, blue, 1.0f );

    // töröljük a frampuffert (GL_COLOR_BUFFER_BIT)...
    // ... és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

}

void App::RenderGUI() {
    if ( ImGui::Begin( "Teszt" ) ) {
        ImGui::DragFloat( "red", &red, 0.001f, 0.0f, 1.0f );
    }
    ImGui::End();
}

void App::KeyboardDown( const SDL_KeyboardEvent& key ) {
}

void App::KeyboardUp( const SDL_KeyboardEvent& key ) {
}

void App::MouseMotion( const SDL_MouseMotionEvent& mouse ) {
    green = (float) mouse.x / x_resolution;
    blue = (float) mouse.y / y_resolution;
    //red += mouse.xrel * 0.01f;
}

void App::MouseDown( const SDL_MouseButtonEvent& mouse ) {
}

void App::MouseUp( const SDL_MouseButtonEvent& mouse ) {
}

void App::MouseWheel( const SDL_MouseWheelEvent& wheel ) {
}


void App::Resize( int w, int h ) {
    glViewport( 0, 0, w, h );
    x_resolution = w;
    y_resolution = h;
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
