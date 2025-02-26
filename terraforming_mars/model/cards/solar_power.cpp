#include "solar_power.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

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
