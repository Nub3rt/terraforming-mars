#include "zeppelins.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
Zeppelins::Zeppelins( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::ZEPPELINS, 13, false, false ) {}

Zeppelins::~Zeppelins() noexcept {}

bool Zeppelins::SatisfiesRequirements( const GameModel& _model ) const {
    return _model.Oxygen() >= 5;
}

void Zeppelins::ApplyImmediateEffects( const GameModel& _model ) {
    _owner->GainCreditProduction( _model.CityCount() );
}

int Zeppelins::DoCountVPs( const GameModel& _model ) const {
    return 1;
}
}
