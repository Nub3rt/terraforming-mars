#pragma once

#include <glm/glm.hpp>

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

    glm::vec2 pos = glm::vec2( 0.0f, -0.8f );
    glm::vec2 scale = glm::vec2( 0.25f, 0.25f );
    bool hovered = false;

private:
    model::decks::Card* _card;
};
}
