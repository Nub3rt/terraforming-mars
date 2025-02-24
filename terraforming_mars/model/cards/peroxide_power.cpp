#include "peroxide_power.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
PeroxidePower::PeroxidePower( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::PEROXIDE_POWER, 7, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

PeroxidePower::~PeroxidePower() noexcept {}

bool PeroxidePower::SatisfiesRequirements() const {
    return _owner->GetResourceProduction( Resource::CREDIT ) >= 1;
}

void PeroxidePower::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::CREDIT, 1 );

    _owner->GainResourceProduction( Resource::ENERGY, 2 );
}
}
