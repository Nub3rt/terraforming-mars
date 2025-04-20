#pragma once

#include <SDL3/SDL.h>

#include "gl_utils/gl_utils.hpp"

namespace view
{
class View
{
public:
    virtual ~View() {}

    virtual void Clean() {}

    virtual void Update( const UpdateInfo& update_info ) {}
    virtual void Render() {}
    virtual void RenderGUI() {}

    virtual void KeyboardDown( const SDL_KeyboardEvent& key ) {}
    virtual void KeyboardUp( const SDL_KeyboardEvent& key ) {}
    virtual void MouseMotion( const SDL_MouseMotionEvent& mouse ) {}
    virtual void MouseDown( const SDL_MouseButtonEvent& mouse ) {}
    virtual void MouseUp( const SDL_MouseButtonEvent& mouse ) {}
    virtual void MouseWheel( const SDL_MouseWheelEvent& wheel ) {}
    virtual void Resize( int w, int h ) {}

    virtual void OtherEvent( const SDL_Event& event ) {}

protected:
    View() {}
};
}
