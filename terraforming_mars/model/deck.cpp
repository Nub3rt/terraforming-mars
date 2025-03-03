#pragma once

#include "deck.h"

#include <algorithm>
#include <vector>
#include <random>

#include "card.h"
#include "game_model.h"

namespace model::decks
{
Deck::Deck( const GameModel& model, DeckProvider* deck_provider, int seed ) : _deck( deck_provider->GenerateDeck( model ) ),
    _discard_pile(), _random( seed ) {
    std::shuffle( _deck.begin(), _deck.end(), _random );
}

Deck::~Deck() {
    for ( Card* card : _deck )
        delete card;

    for ( Card* card : _discard_pile )
        delete card;
}

Card* Deck::DrawCard() {
    if ( _deck.size() == 0 ) {
        if ( _discard_pile.size() == 0 )
            return nullptr;

        _deck.swap( _discard_pile );
        std::shuffle( _deck.begin(), _deck.end(), _random );
    }

    Card* card = _deck.back();
    _deck.pop_back();
    return card;
}

void Deck::DiscardCard( Card* card ) {
    _discard_pile.push_back( card );
}
}
