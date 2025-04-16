#include "solar_power.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
SolarPower::SolarPower( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::SOLAR_POWER, 11 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

SolarPower::~SolarPower() noexcept {}

void SolarPower::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 1 );
}

int SolarPower::DoCountVPs() const {
    return 1;
}
}
