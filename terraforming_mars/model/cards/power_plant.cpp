#include "power_plant.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
PowerPlant::PowerPlant( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::POWER_PLANT, 4, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

PowerPlant::~PowerPlant() noexcept {}

void PowerPlant::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 1 );
}
}
