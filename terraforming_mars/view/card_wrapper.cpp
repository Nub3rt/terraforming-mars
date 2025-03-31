#include "card_wrapper.h"

#include <glm/gtx/easing.hpp>

#include "../model/decks/card.h"

namespace view
{
CardWrapper::CardWrapper( model::decks::Card* card ) : _card( card ) {
    pos = base_pos;
    scale = base_scale;
    rotate = base_rotate;

    pos.SetEase( glm::sineEaseInOut<float> );
    scale.SetEase( glm::sineEaseInOut<float> );
    rotate.SetEase( glm::sineEaseInOut<float> );
}

CardWrapper::~CardWrapper() {}

void CardWrapper::GoToBase( float duration ) {
    pos.SetAnim( *pos, base_pos, duration );
    scale.SetAnim( *scale, base_scale, duration );
    rotate.SetAnim( *rotate, base_rotate, duration );
}
void CardWrapper::XGoToBase( float duration ) {
    pos.x.SetAnim( pos->x, base_pos.x, duration );
}
}
