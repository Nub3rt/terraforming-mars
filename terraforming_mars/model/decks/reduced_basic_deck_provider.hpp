#pragma once

#include <vector>

#include "card.hpp"
#include "deck_provider.hpp"
#include "../game_model.hpp"

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
