#include "worms.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
Worms::Worms( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::WORMS, 8, false, false ) {
    AddTag( Tag::MICROBE );
}

Worms::~Worms() noexcept {}

bool Worms::SatisfiesRequirements( const GameModel& _model ) const {
    return _model.Oxygen() >= 4;
}

void Worms::ApplyImmediateEffects( const GameModel& _model ) {
    _owner->GainPlantsProduction( _owner->GetTagCount( Tag::MICROBE ) / 2 );
}
}
