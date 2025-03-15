#pragma once

#include <vector>

#include "card.h"
#include "deck_provider.h"
#include "../game_model.h"

namespace model::decks
{
class ReducedBasicDeckProvider : public DeckProvider
{
public:
    ReducedBasicDeckProvider() noexcept;
    ~ReducedBasicDeckProvider() noexcept;

    std::vector<Card*> GenerateDeck( const GameModel& model ) override;
};
}
