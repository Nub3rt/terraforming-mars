#include "fusion_power.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
FusionPower::FusionPower( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::FUSION_POWER, 14 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
    AddTag( Tag::SCIENCE );
}

FusionPower::~FusionPower() noexcept {}

bool FusionPower::SatisfiesRequirements() const {
    return _owner->GetTagCount( Tag::POWER ) >= 2;
}

void FusionPower::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 3 );
}
}
