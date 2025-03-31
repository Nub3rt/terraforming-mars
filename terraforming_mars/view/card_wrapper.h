#pragma once

#include <glm/glm.hpp>

#include "constants.h"

#include "../model/decks/card.h"

namespace view
{
class CardWrapper
{
public:
    CardWrapper( model::decks::Card* card );
    ~CardWrapper();

    inline model::decks::Card* operator->() noexcept { return _card; }
    inline model::decks::Card* operator*() noexcept { return _card; }

    glm::vec2 pos = glm::vec2( 0.0f, HAND_BASE_Y );
    glm::vec2 scale = CARD_BASE_SCALE;
    float rotate = 0.0f;
    bool hovered = false;

private:
    model::decks::Card* _card;
};
}
