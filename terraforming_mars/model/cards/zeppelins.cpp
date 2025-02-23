#include "zeppelins.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
Zeppelins::Zeppelins() noexcept :
    AutomatedCard( CardID::ZEPPELINS, 13, false, false ) {}

Zeppelins::~Zeppelins() noexcept {}

bool Zeppelins::SatisfiesRequirements( const GameModel& model ) const {
    return model.Oxygen() >= 5;
}

void Zeppelins::ApplyImmediateEffects( const GameModel& model ) {
    _owner->GainCreditProduction( model.CityCount() );
}

int Zeppelins::DoCountVPs( const GameModel& model ) const {
    return 1;
}
}
