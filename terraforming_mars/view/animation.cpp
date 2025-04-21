#include "animation.hpp"

#include <stdexcept>
#include <string>

#include <glm/glm.hpp>

#include "game_view.hpp"

namespace view
{
#pragma region Animation

Animation::Animation( float lockout_time ) : _lockout_time( lockout_time ) {}

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

#pragma region Animation

#pragma region InstantAnimation

InstantAnimation::InstantAnimation( std::function<void()> perform )
    : Animation(), perform( perform ) {}

InstantAnimation::InstantAnimation( float lockout_time, std::function<void()> perform )
    : Animation( lockout_time ), perform( perform ) {}

void InstantAnimation::Render( GameView* view ) {
    if ( _performed )
        return;

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

void TextAnimation::Render( GameView* view ) {
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

CardAnimation::CardAnimation( float duration, CardWrapper* card, glm::vec2 end_pos,
    float end_scale, float end_rotate ) : Animation(), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos(), start_scale(), start_rotate() {
}

CardAnimation::CardAnimation( float lockout_time, float duration, CardWrapper* card, glm::vec2 end_pos,
    float end_scale, float end_rotate ) : Animation( lockout_time ), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos(), start_scale(), start_rotate() {
}

CardAnimation::CardAnimation( float duration, CardWrapper* card, glm::vec2 start_pos, glm::vec2 end_pos,
    float end_scale, float end_rotate ) : Animation(), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos( start_pos ), start_scale(), start_rotate() {
}

CardAnimation::CardAnimation( float lockout_time, float duration, CardWrapper* card, glm::vec2 start_pos, glm::vec2 end_pos,
    float end_scale, float end_rotate ) : Animation( lockout_time ), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos( start_pos ), start_scale(), start_rotate() {
}

CardAnimation::CardAnimation( float duration, CardWrapper* card, glm::vec2 start_pos, glm::vec2 end_pos,
    float start_scale, float end_scale, float end_rotate ) : Animation(), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos( start_pos ), start_scale( start_scale ), start_rotate() {
}

CardAnimation::CardAnimation( float lockout_time, float duration, CardWrapper* card, glm::vec2 start_pos, glm::vec2 end_pos,
    float start_scale, float end_scale, float end_rotate ) : Animation( lockout_time ), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos( start_pos ), start_scale( start_scale ), start_rotate() {
}

CardAnimation::CardAnimation( float duration, CardWrapper* card, glm::vec2 start_pos, glm::vec2 end_pos,
    float start_scale, float end_scale, float start_rotate, float end_rotate ) : Animation(), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos( start_pos ), start_scale( start_scale ), start_rotate( start_rotate ) {
}

CardAnimation::CardAnimation( float lockout_time, float duration, CardWrapper* card, glm::vec2 start_pos, glm::vec2 end_pos,
    float start_scale, float end_scale, float start_rotate, float end_rotate ) : Animation( lockout_time ), duration( duration ), card( card ),
    end_pos( end_pos ), end_scale( end_scale ), end_rotate( end_rotate ),
    start_pos( start_pos ), start_scale( start_scale ), start_rotate( start_rotate ) {}

void CardAnimation::Render( GameView* view ) {
    if ( _performed )
        return;

    view->RenderAnimation( this );
    _performed = true;
}

bool CardAnimation::IsOver() const noexcept {
    return _performed;
}

void CardAnimation::DoUpdate( float delta ) {}

#pragma endregion CardAnimation

#pragma region CardDrawAnimation

CardDrawAnimation::CardDrawAnimation( CardWrapper* card )
    : Animation( CARD_DRAW_LOCKOUT_DURATION ), card( card ) {}

CardDrawAnimation::CardDrawAnimation( CardWrapper* card, float speed )
    : CardDrawAnimation( card ) {
    this->speed = speed;
    _lockout_time /= speed;
}

CardDrawAnimation::~CardDrawAnimation() {
    if ( owns_card )
        delete card;
}

void CardDrawAnimation::Render( GameView* view ) {
    view->RenderAnimation( this );
}

bool CardDrawAnimation::IsOver() const noexcept {
    return elapsed >= CARD_DRAW_TOTAL_DURATION;
}

void CardDrawAnimation::DoUpdate( float delta ) {
    elapsed = fminf( elapsed + delta * speed, CARD_DRAW_TOTAL_DURATION );
}

#pragma endregion CardDrawAnimation

#pragma region CardPlayAnimation

CardPlayAnimation::CardPlayAnimation( CardWrapper* card, glm::vec2 start_pos, glm::vec2 end_pos )
    : Animation( CARD_PLAY_LOCKOUT_DURATION ), card( card ), start_pos( start_pos ), end_pos( end_pos ) {}

CardPlayAnimation::~CardPlayAnimation() {
    if ( owns_card )
        delete card;
}

void CardPlayAnimation::Render( GameView* view ) {
    view->RenderAnimation( this );
}

bool CardPlayAnimation::IsOver() const noexcept {
    return elapsed >= CARD_PLAY_TOTAL_DURATION;
}

void CardPlayAnimation::DoUpdate( float delta ) {
    elapsed = fminf( elapsed + delta, CARD_PLAY_TOTAL_DURATION );
}

#pragma endregion CardPlayAnimation

#pragma region SequentialAnimation

SequentialAnimation::SequentialAnimation( Animation* first, Animation* second )
    : Animation( first->_lockout_time + second->_lockout_time ),
    _first( first ), _second( second ) {}

SequentialAnimation::SequentialAnimation( float lockout_time, Animation* first, Animation* second )
    : Animation( lockout_time ), _first( first ), _second( second ) {}

void SequentialAnimation::Render( GameView* view ) {
    if ( !_first->IsOver() )
        _first->Render( view );
    else
        _second->Render( view );
}

bool SequentialAnimation::IsOver() const noexcept {
    return _second->IsOver();
}

void SequentialAnimation::DoUpdate( float delta ) {
    if ( !_first->IsOver() )
        _first->DoUpdate( delta );
    else
        _second->DoUpdate( delta );
}

#pragma endregion SequentialAnimation
}
