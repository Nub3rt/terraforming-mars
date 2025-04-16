#include "nuclear_power.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
NuclearPower::NuclearPower( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::NUCLEAR_POWER, 10 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

NuclearPower::~NuclearPower() noexcept {}

bool NuclearPower::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::CREDIT ) >= 2 - 5;
}

void NuclearPower::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::CREDIT, 2 );

    _owner->GainResourceProduction( Resource::ENERGY, 3 );
}
}
