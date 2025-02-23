#include "insects.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
Insects::Insects( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::INSECTS, 9, false, false ) {
    AddTag( Tag::MICROBE );
}

Insects::~Insects() noexcept {}

bool Insects::SatisfiesRequirements( const GameModel& _model ) const {
    return _model.Oxygen() >= 6;
}

void Insects::ApplyImmediateEffects( const GameModel& _model ) {
    _owner->GainPlantsProduction( _owner->GetTagCount( Tag::PLANT ) );
}
}
