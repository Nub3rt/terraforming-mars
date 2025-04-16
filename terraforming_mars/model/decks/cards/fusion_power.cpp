#include "fusion_power.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
    return _holder->GetTagCount( Tag::POWER ) >= 2;
}

void FusionPower::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 3 );
}
}
