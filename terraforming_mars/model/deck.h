#pragma once

#include "deck.fwd.h"
#include "game_model.fwd.h"

#include <vector>
#include <random>

#include "card.h"
#include "deck_provider.h"

namespace model::decks
{
class Deck
{
public:
    Deck( const GameModel& model, DeckProvider* deck_provider, int seed );
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
