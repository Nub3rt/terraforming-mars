#include "animation.h"

#include <stdexcept>
#include <string>

#include <glm/glm.hpp>

#include "view.h"

namespace view
{
Animation::Animation( float lockout_time )
    : _lockout_time( lockout_time ) {
}

void Animation::Update( float delta ) {
    if ( _lockout_time != 0.0f ) {
        if ( _lockout_time > delta ) {
            _lockout_time -= delta;
        } else {
            _lockout_time = 0.0f;
        }
    }

    DoUpdate( delta );
}

#pragma region InstantAnimation

InstantAnimation::InstantAnimation( std::function<void()> perform )
    : Animation(), perform( perform ) {}

InstantAnimation::InstantAnimation( float lockout_time, std::function<void()> perform )
    : Animation( lockout_time ), perform( perform ) {}

void InstantAnimation::Render( View* view ) {
    if ( _performed )
        throw std::logic_error( "InstantAnimation::Render: instant animation was already performed!" );

    view->Render( this );
    _performed = true;
}

bool InstantAnimation::IsOver() const noexcept {
    return _performed;
}

void InstantAnimation::DoUpdate( float delta ) {}

#pragma endregion InstantAnimation

#pragma region TextAnimation

TextAnimation::TextAnimation( float duration, std::string text, glm::vec2 start_pos, glm::vec2 end_pos,
    glm::vec4 start_color, float start_scale ) : Animation(), text( text ), pos( start_pos ), color( start_color ), scale( start_scale ) {
    pos.UpdateAnim( end_pos, duration );
}

TextAnimation::TextAnimation( float lockout_time, float duration, std::string text, glm::vec2 start_pos, glm::vec2 end_pos,
    glm::vec4 start_color, float start_scale ) : Animation( lockout_time ), text( text ), pos( start_pos ), color( start_color ), scale( start_scale ) {
    pos.UpdateAnim( end_pos, duration );
}

TextAnimation::TextAnimation( float duration, std::string text, glm::vec2 start_pos, glm::vec2 end_pos,
    glm::vec4 start_color, glm::vec4 end_color, float start_scale ) : Animation(), text( text ), pos( start_pos ), color( start_color ), scale( start_scale ) {
    pos.UpdateAnim( end_pos, duration );
    color.UpdateAnim( end_color, duration );
}

TextAnimation::TextAnimation( float lockout_time, float duration, std::string text, glm::vec2 start_pos, glm::vec2 end_pos,
    glm::vec4 start_color, glm::vec4 end_color, float start_scale ) : Animation( lockout_time ), text( text ), pos( start_pos ), color( start_color ), scale( start_scale ) {
    pos.UpdateAnim( end_pos, duration );
    color.UpdateAnim( end_color, duration );
}

TextAnimation::TextAnimation( float duration, std::string text, glm::vec2 start_pos, glm::vec2 end_pos,
    glm::vec4 start_color, glm::vec4 end_color, float start_scale, float end_scale ) : Animation(), text( text ), pos( start_pos ), color( start_color ), scale( start_scale ) {
    pos.UpdateAnim( end_pos, duration );
    color.UpdateAnim( end_color, duration );
    scale.UpdateAnim( end_scale, duration );
}

TextAnimation::TextAnimation( float lockout_time, float duration, std::string text, glm::vec2 start_pos, glm::vec2 end_pos,
    glm::vec4 start_color, glm::vec4 end_color, float start_scale, float end_scale ) : Animation( lockout_time ), text( text ), pos( start_pos ), color( start_color ), scale( start_scale ) {
    pos.UpdateAnim( end_pos, duration );
    color.UpdateAnim( end_color, duration );
    scale.UpdateAnim( end_scale, duration );
}

void TextAnimation::Render( View* view ) {
    view->Render( this );
}

bool TextAnimation::IsOver() const noexcept {
    return !pos.Animating() && !color.Animating() && ! scale.Animating();
}

void TextAnimation::DoUpdate( float delta ) {
    pos.Update( delta );
    color.Update( delta );
    scale.Update( delta );
}

#pragma endregion TextAnimation
}