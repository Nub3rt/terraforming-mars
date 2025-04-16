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
    enum class State
    {
        IDLE,
        HOVERED,
        DRAGGING,
        DRAWING,
        TO_HAND,
    };

    enum class Visual
    {
        NONE,
        HIGHLIGHT,
        ACTION_HIGHLIGHT,
        SELL_HIGHLIGHT,
        FADED,
    };

    CardWrapper( const model::decks::Card* card );
    ~CardWrapper();

    inline const model::decks::Card* operator->() noexcept { return _card; }
    inline const model::decks::Card* operator*() noexcept { return _card; }

    void SetEase( Animatable<float>::ease_t );
    void SetDefaultEase();

    void Update( float delta );

    void GoToBase( float duration );

    State state = State::IDLE;
    Visual visual = Visual::NONE;

    glm::vec2 base_pos = CARD_DRAW_POS_START;
    float base_scale = CARD_BASE_SCALE;
    float base_rotate = 0.0f;

    Animatable<glm::vec2> pos;
    Animatable<float> scale;
    Animatable<float> rotate;

private:
    const model::decks::Card* _card;
};
}
