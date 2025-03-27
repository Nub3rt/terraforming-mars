#include <GL/glew.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengl.h>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

#include "app.h"

int main( int argc, char* argv[] ) {
    SDL_SetLogPriority( SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR );

    if ( !SDL_Init( SDL_INIT_VIDEO ) ) {
        SDL_LogError( SDL_LOG_CATEGORY_ERROR, "[SDL initialization] Error during the SDL initialization: %s", SDL_GetError() );
        return 1;
    }

    std::atexit( SDL_Quit );


    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

#ifdef _DEBUG
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG );
#endif _DEBUG

    SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );


    SDL_Window* window = SDL_CreateWindow( "Terraforming Mars",
                                           1600,
                                           900,
                                           SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );

    if ( window == nullptr ) {
        SDL_LogError( SDL_LOG_CATEGORY_ERROR, "[Window creation] Error during the SDL initialization: %s", SDL_GetError() );
        return 1;
    }


    SDL_GLContext context = SDL_GL_CreateContext( window );

    if ( context == nullptr ) {
        SDL_LogError( SDL_LOG_CATEGORY_ERROR, "[OGL context creation] Error during the creation of the OGL context: %s", SDL_GetError() );
        return 1;
    }

    SDL_GL_SetSwapInterval( 1 );

    GLenum error = glewInit();

    if ( error != GLEW_OK ) {
        SDL_LogError( SDL_LOG_CATEGORY_ERROR, "[GLEW] Error during the initialization of glew." );
        return 1;
    }

    int gl_version[ 2 ] = { -1, -1 };
    glGetIntegerv( GL_MAJOR_VERSION, &gl_version[ 0 ] );
    glGetIntegerv( GL_MINOR_VERSION, &gl_version[ 1 ] );

    if ( gl_version[ 0 ] == -1 || gl_version[ 1 ] == -1 ) {
        SDL_GL_DestroyContext( context );
        SDL_DestroyWindow( window );

        SDL_LogError( SDL_LOG_CATEGORY_ERROR, "[OGL context creation] Error during the inialization of the OGL context! Maybe one of the SDL_GL_SetAttribute(...) calls is erroneous." );
        return 1;
    }

    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Running OpenGL %d.%d", gl_version[ 0 ], gl_version[ 1 ] );


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForOpenGL( window, context );
    ImGui_ImplOpenGL3_Init();


    view::App app;

    if ( !app.Init() ) {
        SDL_GL_DestroyContext( context );
        SDL_DestroyWindow( window );
        SDL_LogError( SDL_LOG_CATEGORY_ERROR, "[app.Init] Error during the initialization of the application!" );
        return 1;
    }


    {
        bool quit = false;
        bool show_imgui = true;

        SDL_Event event;

        while ( !quit ) {
            while ( SDL_PollEvent( &event ) ) {
                ImGui_ImplSDL3_ProcessEvent( &event );
                bool was_mouse_captured = ImGui::GetIO().WantCaptureMouse;
                bool was_keyboard_captured = ImGui::GetIO().WantCaptureKeyboard;

                switch ( event.type ) {
                    case SDL_EVENT_QUIT:
                        quit = true;
                        break;

                    case SDL_EVENT_KEY_DOWN:
                        if ( event.key.key == SDLK_F11 ) {
                            if ( !event.key.repeat ) {
                                SDL_WindowFlags in_fullscreen = SDL_GetWindowFlags( window ) & SDL_WINDOW_FULLSCREEN;
                                SDL_SetWindowFullscreen( window, !in_fullscreen );
                            }
                            was_keyboard_captured = true;
                        }
                        if ( event.key.key == SDLK_F1 &&
                             event.key.mod & SDL_KMOD_CTRL &&
                             !(event.key.mod & (SDL_KMOD_SHIFT | SDL_KMOD_ALT | SDL_KMOD_GUI)) ) {
                            if ( !event.key.repeat ) {
                                show_imgui = !show_imgui;
                            }
                            was_keyboard_captured = true;
                        }

                        if ( !was_keyboard_captured )
                            app.KeyboardDown( event.key );
                        break;
                    case SDL_EVENT_KEY_UP:
                        if ( !was_keyboard_captured )
                            app.KeyboardUp( event.key );
                        break;

                    case SDL_EVENT_MOUSE_BUTTON_DOWN:
                        if ( !was_mouse_captured )
                            app.MouseDown( event.button );
                        break;
                    case SDL_EVENT_MOUSE_BUTTON_UP:
                        if ( !was_mouse_captured )
                            app.MouseUp( event.button );
                        break;
                    case SDL_EVENT_MOUSE_WHEEL:
                        if ( !was_mouse_captured )
                            app.MouseWheel( event.wheel );
                        break;
                    case SDL_EVENT_MOUSE_MOTION:
                        if ( !was_mouse_captured )
                            app.MouseMotion( event.motion );
                        break;

                    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                        int w, h;
                        SDL_GetWindowSize( window, &w, &h );
                        app.Resize( w, h );
                        break;

                    default:
                        app.OtherEvent( event );
                }
            }

            static Uint64 last_tick = SDL_GetTicks();
            Uint64 current_tick = SDL_GetTicks();
            view::UpdateInfo update_info = { current_tick / 1000.0f, (current_tick - last_tick) / 1000.0f };
            last_tick = current_tick;

            app.Update( update_info );
            app.Render();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();

            ImGui::NewFrame();
            if ( show_imgui )
                app.RenderGUI();
            ImGui::Render();

            ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
            SDL_GL_SwapWindow( window );
        }

        app.Clean();
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext( context );
    SDL_DestroyWindow( window );

    return 0;
}
