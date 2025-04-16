#pragma once

#include <vector>

#include "card.hpp"
#include "../game_model.hpp"

namespace model::decks
{
class DeckProvider
{
public:
    virtual ~DeckProvider() noexcept;

    virtual std::vector<Card*> GenerateDeck( const GameModel& ) = 0;

protected:
    DeckProvider() noexcept;
};
}
