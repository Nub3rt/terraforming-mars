#include "deep_well_heating.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
