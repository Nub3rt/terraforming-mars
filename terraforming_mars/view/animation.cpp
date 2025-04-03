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

    view->RenderAnimation( this );
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
    view->RenderAnimation( this );
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

#pragma region CardAnimation


CardAnimation::CardAnimation( float duration, const model::decks::Card* card, glm::vec2 end_pos,
    float end_scale, float end_rotate ) : Animation(), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos(), start_scale(), start_rotate() {
}

CardAnimation::CardAnimation( float lockout_time, float duration, const model::decks::Card* card, glm::vec2 end_pos,
    float end_scale, float end_rotate ) : Animation( lockout_time ), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos(), start_scale(), start_rotate() {
}

CardAnimation::CardAnimation( float duration, const model::decks::Card* card, glm::vec2 start_pos, glm::vec2 end_pos,
    float end_scale, float end_rotate ) : Animation(), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos( start_pos ), start_scale(), start_rotate() {
}

CardAnimation::CardAnimation( float lockout_time, float duration, const model::decks::Card* card, glm::vec2 start_pos, glm::vec2 end_pos,
    float end_scale, float end_rotate ) : Animation( lockout_time ), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos( start_pos ), start_scale(), start_rotate() {
}

CardAnimation::CardAnimation( float duration, const model::decks::Card* card, glm::vec2 start_pos, glm::vec2 end_pos,
    float start_scale, float end_scale, float end_rotate ) : Animation(), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos( start_pos ), start_scale( start_scale ), start_rotate() {
}

CardAnimation::CardAnimation( float lockout_time, float duration, const model::decks::Card* card, glm::vec2 start_pos, glm::vec2 end_pos,
    float start_scale, float end_scale, float end_rotate ) : Animation( lockout_time ), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos( start_pos ), start_scale( start_scale ), start_rotate() {
}

CardAnimation::CardAnimation( float duration, const model::decks::Card* card, glm::vec2 start_pos, glm::vec2 end_pos,
    float start_scale, float end_scale, float start_rotate, float end_rotate ) : Animation(), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos( start_pos ), start_scale( start_scale ), start_rotate( start_rotate ) {
}

CardAnimation::CardAnimation( float lockout_time, float duration, const model::decks::Card* card, glm::vec2 start_pos, glm::vec2 end_pos,
    float start_scale, float end_scale, float start_rotate, float end_rotate ) : Animation( lockout_time ), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos( start_pos ), start_scale( start_scale ), start_rotate( start_rotate ) {}

void CardAnimation::Render( View* view ) {
    if ( _performed )
        return;

    view->RenderAnimation( this );
    _performed = true;
}

bool CardAnimation::IsOver() const noexcept {
    return _performed;
}

void CardAnimation::DoUpdate( float delta ) {}

#pragma region CardAnimation
}
