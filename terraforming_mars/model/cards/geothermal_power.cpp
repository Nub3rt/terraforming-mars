#include "geothermal_power.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
GeothermalPower::GeothermalPower( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::GEOTHERMAL_POWER, 11, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

GeothermalPower::~GeothermalPower() noexcept {}

void GeothermalPower::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 2 );
}
}
