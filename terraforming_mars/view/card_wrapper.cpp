#include "card_wrapper.h"

#include <glm/gtx/easing.hpp>

#include "../model/decks/card.h"

namespace view
{
CardWrapper::CardWrapper( const model::decks::Card* card ) : _card( card ) {
    pos = base_pos;
    scale = base_scale;
    rotate = base_rotate;

    SetDefaultEase();
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
    pos.Update( delta );
    scale.Update( delta );
    rotate.Update( delta );
}

void CardWrapper::GoToBase( float duration ) {
    pos.SetAnim( *pos, base_pos, duration );
    scale.SetAnim( *scale, base_scale, duration );
    rotate.SetAnim( *rotate, base_rotate, duration );
}
}
