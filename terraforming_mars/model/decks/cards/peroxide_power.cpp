#include "peroxide_power.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
PeroxidePower::PeroxidePower( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::PEROXIDE_POWER, 7 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

PeroxidePower::~PeroxidePower() noexcept {}

bool PeroxidePower::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::CREDIT ) >= 1 - 5;
}

void PeroxidePower::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::CREDIT, 1 );

    _owner->GainResourceProduction( Resource::ENERGY, 2 );
}
}
