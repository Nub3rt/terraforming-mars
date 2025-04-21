#include "menu_view.hpp"

#include <format>
#include <functional>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/easing.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <GL/glew.h>

#include "animatable.hpp"
#include "constants.hpp"
#include "view.hpp"

#include "gl_utils/camera.hpp"
#include "gl_utils/gl_utils.hpp"

namespace view
{
MenuView::MenuView() : View(), _builder() {
    static const std::function<float( float )> ease = []( float t ) {
        return 4.0f * (t - 0.5f) * (t - 0.5f);
    };
    _eye.x.SetEase( ease );
    _eye.z.SetEase( glm::linearInterpolation<float> );
    _at.x.SetEase( ease );
    _at.z.SetEase( glm::linearInterpolation<float> );
}

MenuView::~MenuView() {}

bool MenuView::Init( Camera* camera ) {
    _camera = camera;
    _camera->SetView(
        *_eye,
        *_at,
        CAMERA_WORLD_UP
    );

    InitShaders();
    InitGeometry();
    InitTextures();

    return true;
}

void MenuView::Clean() {
    CleanShaders();
    CleanGeometry();
    CleanTextures();
}

void MenuView::Update( const UpdateInfo& update_info ) {
    if ( _transitioning && !_eye.Animating() ) {
        _transitioning = false;
        _can_continue = _game_view != nullptr;
        _enter_game.Invoke();
    }

    _eye.Update( update_info.delta );
    _at.Update( update_info.delta );

    _camera->SetView(
        *_eye,
        *_at,
        CAMERA_WORLD_UP
    );
}

void MenuView::Render() {
    glUseProgram( _program_pane_id );
    glBindVertexArray( _rectangle_gpu.vao_id );

    glActiveTexture( GL_TEXTURE0 );
    glUniform1i( ul( "image" ), 0 );

    static const glm::mat4 horizontal_translate = glm::translate( glm::vec3( MENU_X, 0.0f, MENU_Z ) );
    glm::mat4 view_proj = _camera->GetViewProj();

    // title
    {
        static const float ratio = (float)_tm_texture.width / _tm_texture.height;
        static const glm::mat4 vertical_translate = glm::translate( glm::vec3( 0.0f, MENU_TM_Y, 0.0f ) );
        static const glm::mat4 scale = glm::scale( glm::vec3( ratio, 1.0f, 1.0f ) );

        glBindTexture( GL_TEXTURE_2D, _tm_texture.id );

        glm::mat4 world = view_proj * vertical_translate * horizontal_translate * scale;
        glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

        glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );
    }

    // continue
    if ( _can_continue ) {
        static const float ratio = (float)_continue_texture.width / _continue_texture.height;
        static const glm::mat4 vertical_translate = glm::translate( glm::vec3( 0.0f, MENU_CONTINUE_Y, 0.0f ) );
        glm::vec3 scale = glm::vec3( ratio, 1.0f, 1.0f );
        scale *= MENU_BUTTON_SCALE;
        if ( !_transitioning && _mouse_hover_stencil == STENCIL_MENU_CONTINUE )
            scale *= MOUSE_HOVER_SIZE_MULTIPLIER;
        glm::mat4 scale_transform = glm::scale( scale );

        glBindTexture( GL_TEXTURE_2D, _continue_texture.id );

        glm::mat4 world = view_proj * vertical_translate * horizontal_translate * scale_transform;
        glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

        SetStencilRef( STENCIL_MENU_CONTINUE );

        glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );
    }

    // new game
    {
        static const float ratio = (float)_new_game_texture.width / _new_game_texture.height;
        static const glm::mat4 vertical_translate = glm::translate( glm::vec3( 0.0f, MENU_NEW_GAME_Y, 0.0f ) );
        glm::vec3 scale = glm::vec3( ratio, 1.0f, 1.0f );
        scale *= MENU_BUTTON_SCALE;
        if ( !_transitioning && _mouse_hover_stencil == STENCIL_MENU_NEW_GAME )
            scale *= MOUSE_HOVER_SIZE_MULTIPLIER;
        glm::mat4 scale_transform = glm::scale( scale );

        glBindTexture( GL_TEXTURE_2D, _new_game_texture.id );

        glm::mat4 world = view_proj * vertical_translate * horizontal_translate * scale_transform;
        glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

        SetStencilRef( STENCIL_MENU_NEW_GAME );

        glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );
    }

    // quit
    {
        static const float ratio = (float)_quit_texture.width / _quit_texture.height;
        static const glm::mat4 vertical_translate = glm::translate( glm::vec3( 0.0f, MENU_QUIT_Y, 0.0f ) );
        glm::vec3 scale = glm::vec3( ratio, 1.0f, 1.0f );
        scale *= MENU_BUTTON_SCALE;
        if ( !_transitioning && _mouse_hover_stencil == STENCIL_MENU_QUIT )
            scale *= MOUSE_HOVER_SIZE_MULTIPLIER;
        glm::mat4 scale_transform = glm::scale( scale );

        glBindTexture( GL_TEXTURE_2D, _quit_texture.id );

        glm::mat4 world = view_proj * vertical_translate * horizontal_translate * scale_transform;
        glUniformMatrix4fv( ul( "world" ), 1, GL_FALSE, glm::value_ptr( world ) );

        SetStencilRef( STENCIL_MENU_QUIT );

        glDrawElements( GL_TRIANGLES, _rectangle_gpu.count, GL_UNSIGNED_INT, nullptr );
    }


    SetStencilRef();

    glBindTexture( GL_TEXTURE_2D, 0 );

    glBindVertexArray( 0 );
    glUseProgram( 0 );


    if ( _transitioning )
        _game_view->RenderMars();
}

void MenuView::KeyboardDown( const SDL_KeyboardEvent& key ) {
}

void MenuView::KeyboardUp( const SDL_KeyboardEvent& key ) {
}

void MenuView::MouseMotion( const SDL_MouseMotionEvent& mouse ) {
    _mouse_hover_stencil = GetStencilValue( mouse.x, mouse.y );
}

void MenuView::MouseDown( const SDL_MouseButtonEvent& mouse ) {
    _mouse_down_stencil = GetStencilValue( mouse.x, mouse.y );
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "MenuView::MouseDown: Stencil value: %d", _mouse_down_stencil );
}

void MenuView::MouseUp( const SDL_MouseButtonEvent& mouse ) {
    uint8_t stencil = GetStencilValue( mouse.x, mouse.y );
    if ( stencil == _mouse_down_stencil ) {
        switch ( stencil ) {
            case STENCIL_NONE: break;
            case 0x00:
                SDL_LogError( SDL_LOG_CATEGORY_ERROR, "MenuView::MouseUp: Invalid stencil value received : 0x00!" );
                break;
            case STENCIL_MENU_CONTINUE:
                TransitionToMars();
                break;
            case STENCIL_MENU_NEW_GAME:
                if ( _debug )
                    _builder.SetSeed( _seed );
                else
                    _builder.SetSeed( rand() );

                _game_view = _builder.SoloGameModel()
                                     .TharsisBoard()
                                     .ReducedBasicDeck()
                                     .SoloGameView()
                                     .GetResult( _camera );

                _game_view->Resize( _width, _height );

                _new_game.Invoke( _game_view );

                TransitionToMars();
                break;
            case STENCIL_MENU_QUIT:
                _quit.Invoke();
                break;
            default:
                SDL_LogError( SDL_LOG_CATEGORY_ERROR, "MenuView::MouseUp: Invalid stencil value received : %#x!", stencil );
                break;
        }
    }
}

void MenuView::Resize( int w, int h ) {
    _width = w;
    _height = h;
}

void MenuView::TransitionToMars() {
    Transition( CAMERA_Z_MENU_EYE, CAMERA_Z_GAME_EYE, CAMERA_Z_MENU_AT, CAMERA_Z_GAME_AT );
}

void MenuView::Transition( float eye_z_from, float eye_z_to, float at_z_from, float at_z_to ) {
    static const float eye_out = (CAMERA_Z_MENU_EYE - CAMERA_Z_GAME_EYE) / 2.0f;
    static const float at_out = (CAMERA_Z_MENU_AT - CAMERA_Z_GAME_AT) / 2.0f;

    _eye.x.SetAnim( CAMERA_X, eye_out, TRANSITION_DURATION );
    _eye.z.SetAnim( eye_z_from, eye_z_to, TRANSITION_DURATION );
    _at.x.SetAnim( CAMERA_X, at_out, TRANSITION_DURATION );
    _at.z.SetAnim( at_z_from, at_z_to, TRANSITION_DURATION );

    _transitioning = true;
}

uint8_t MenuView::GetStencilValue( float mouse_x, float mouse_y ) {
    uint8_t value;
    glReadPixels( (GLint)mouse_x, _height - (GLint)mouse_y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &value );
    return value;
}

void MenuView::InitShaders() {
    _program_pane_id = glCreateProgram();
    AttachShader( _program_pane_id, GL_VERTEX_SHADER, "shaders/rectangle.vert" );
    AttachShader( _program_pane_id, GL_FRAGMENT_SHADER, "shaders/rectangle.frag" );
    LinkProgram( _program_pane_id );
}

void MenuView::CleanShaders() {
    glDeleteProgram( _program_pane_id );
}

void MenuView::InitGeometry() {
    MeshObject<VertexPosTex> rectangle_cpu = {
        std::vector<VertexPosTex> {
            { {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
            { { -1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
            { {  1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f } },
            { { -1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f } },
        },
        std::vector<GLuint> {
            0, 1, 2,
            2, 1, 3,
        }
    };

    _rectangle_gpu = CreateGLObjectFromMesh( rectangle_cpu, vertex_pos_tex_attribute_list );
}

void MenuView::CleanGeometry() {
    CleanOGLObject( _rectangle_gpu );
}

void MenuView::InitTextures() {
    _tm_texture = LoadTexture( "assets/tm_pane.png" );
    _continue_texture = LoadTexture( "assets/continue_pane.png" );
    _new_game_texture = LoadTexture( "assets/new_game_pane.png" );
    _quit_texture = LoadTexture( "assets/quit_pane.png" );
}

void MenuView::CleanTextures() {
    glDeleteTextures( 1, &_tm_texture.id );
    glDeleteTextures( 1, &_continue_texture.id );
    glDeleteTextures( 1, &_new_game_texture.id );
    glDeleteTextures( 1, &_quit_texture.id );
}
}
