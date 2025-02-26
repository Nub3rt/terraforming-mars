#include "deep_well_heating.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
DeepWellHeating::DeepWellHeating( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::DEEP_WELL_HEATING, 13 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

DeepWellHeating::~DeepWellHeating() noexcept {}

void DeepWellHeating::ApplyImmediateEffects() {
    _owner->RaiseTemperature();
    _owner->GainResourceProduction( Resource::ENERGY, 1 );
}
}
