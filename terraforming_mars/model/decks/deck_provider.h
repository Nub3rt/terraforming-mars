#pragma once

#include <vector>

#include "card.h"
#include "../game_model.h"

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
