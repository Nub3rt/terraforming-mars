#pragma once

#include <glm/glm.hpp>

#include "animatable.h"
#include "constants.h"

#include "../model/decks/card.h"

namespace view
{
class CardWrapper
{
public:
    enum State
    {
        IDLE,
        HOVERED,
        DRAGGING,
        DRAWING,
        TO_HAND,
    };

    CardWrapper( model::decks::Card* card, bool drawn = true );
    ~CardWrapper();

    inline model::decks::Card* operator->() noexcept { return _card; }
    inline model::decks::Card* operator*() noexcept { return _card; }

    void SetEase( Animatable<float>::ease_t );
    void SetDefaultEase();

    void Update( float delta );

    void GoToBase( float duration );

    State state = IDLE;

    glm::vec2 base_pos = glm::vec2( 0.0f, HAND_BASE_Y );
    glm::vec2 base_scale = CARD_BASE_SCALE;
    float base_rotate = 0.0f;

    Animatable<glm::vec2> pos;
    Animatable<glm::vec2> scale;
    Animatable<float> rotate;

private:
    model::decks::Card* _card;

    static const glm::vec2 drawing_pos_1;
    static const glm::vec2 drawing_scale_1;
    static const float drawing_rotate_1;
    static const glm::vec2 drawing_pos_2;
    static const glm::vec2 drawing_scale_2;
    static const float drawing_rotate_2;
};
}
