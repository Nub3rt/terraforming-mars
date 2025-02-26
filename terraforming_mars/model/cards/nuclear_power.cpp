#include "nuclear_power.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
NuclearPower::NuclearPower( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::NUCLEAR_POWER, 10 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

NuclearPower::~NuclearPower() noexcept {}

bool NuclearPower::SatisfiesRequirements() const {
    return _owner->GetResourceProduction( Resource::CREDIT ) >= 2 - 5;
}

void NuclearPower::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::CREDIT, 2 );

    _owner->GainResourceProduction( Resource::ENERGY, 3 );
}
}
