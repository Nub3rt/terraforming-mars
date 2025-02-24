#pragma once

#include <vector>
#include <random>

#include "card.h"
#include "deck_provider.h"
#include "game_model.h"

namespace model::decks
{
class Deck
{
public:
    Deck( const GameModel& model, DeckProvider* deck_provider, int seed );
    ~Deck();

    Card* DrawCard();
    void DiscardCard( Card* card );
    
private:
    std::vector<Card*> _deck;
    std::vector<Card*> _discard_pile;

    std::mt19937 _random;
};
}
