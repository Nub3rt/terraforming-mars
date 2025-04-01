#include "card_wrapper.h"

#include <glm/gtx/easing.hpp>

#include "../model/decks/card.h"

namespace view
{
CardWrapper::CardWrapper( model::decks::Card* card, bool drawn ) : _card( card ) {
    if ( !drawn ) {
        pos = base_pos;
        scale = base_scale;
        rotate = base_rotate;

        SetDefaultEase();
        return;
    }

    state = DRAWING_1;
    SetEase( glm::quarticEaseOut<float> );
    pos.SetAnim( drawing_pos_1, drawing_pos_2, CARD_DRAW_DURATION );
    scale.SetAnim( drawing_scale_1, drawing_scale_2, CARD_DRAW_DURATION );
    rotate.SetAnim( drawing_rotate_1, drawing_rotate_2, CARD_DRAW_DURATION );
}

CardWrapper::~CardWrapper() {}


void CardWrapper::SetEase( Animatable<float>::ease_t ease ) {
    pos.SetEase( ease );
    scale.SetEase( ease );
    rotate.SetEase( ease );
}

void CardWrapper::SetDefaultEase() {
    SetEase( glm::sineEaseInOut<float> );
}

void CardWrapper::Update( float delta ) {
    if ( state == DRAWING_1 && !pos.Animating() ) {
        state = DRAWING_2;
        SetEase( glm::quarticEaseIn<float> );
        GoToBase( CARD_DRAW_DURATION * 0.7f );
    }

    if ( state == DRAWING_2 && !pos.Animating() ) {
        state = IDLE;
        SetDefaultEase();
    }

    pos.Update( delta );
    scale.Update( delta );
    rotate.Update( delta );
}

void CardWrapper::GoToBase( float duration ) {
    pos.SetAnim( *pos, base_pos, duration );
    scale.SetAnim( *scale, base_scale, duration );
    rotate.SetAnim( *rotate, base_rotate, duration );
}

const glm::vec2 CardWrapper::drawing_pos_1 = { 1.2f, -0.4f };
const glm::vec2 CardWrapper::drawing_scale_1 = CARD_BASE_SCALE * 1.2f;
const float CardWrapper::drawing_rotate_1 = 0.0f;
const glm::vec2 CardWrapper::drawing_pos_2 = { 0.5f, 0.0f };
const glm::vec2 CardWrapper::drawing_scale_2 = CARD_BASE_SCALE * 1.8f;
const float CardWrapper::drawing_rotate_2 = 0.0f;
}
