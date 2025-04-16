#pragma once

#include "deck.fwd.hpp"
#include "../game_model.fwd.hpp"

#include <vector>
#include <random>

#include "card.hpp"
#include "deck_provider.hpp"

namespace model::decks
{
class Deck
{
public:
    Deck( const GameModel& model, DeckProvider& deck_provider, int seed );
    ~Deck();
    Deck( const Deck& other ) = delete;
    Deck( Deck&& other ) = delete;
    Deck& operator=( const Deck& other ) = delete;
    Deck& operator=( Deck&& other ) = delete;

    Card* DrawCard();
    void DiscardCard( Card* card );
    
private:
    std::vector<Card*> _deck;
    std::vector<Card*> _discard_pile;

    std::mt19937 _random;
};
}
