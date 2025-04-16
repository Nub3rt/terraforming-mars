#include "solar_wind_power.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
SolarWindPower::SolarWindPower( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::SOLAR_WIND_POWER, 11 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::POWER );
    AddTag( Tag::SCIENCE );
}

SolarWindPower::~SolarWindPower() noexcept {}

void SolarWindPower::ApplyImmediateEffects() {
    _owner->GainResource( Resource::TITANIUM, 2 );
    _owner->GainResourceProduction( Resource::ENERGY, 1 );
}
}
