#include "solar_wind_power.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
SolarWindPower::SolarWindPower( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::SOLAR_WIND_POWER, 11, false, true ) {
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
