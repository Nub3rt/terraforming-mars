#include "geothermal_power.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
GeothermalPower::GeothermalPower( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::GEOTHERMAL_POWER, 11 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

GeothermalPower::~GeothermalPower() noexcept {}

void GeothermalPower::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 2 );
}
}
