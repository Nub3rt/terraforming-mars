#include "power_plant.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
PowerPlant::PowerPlant( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::POWER_PLANT, 4 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

PowerPlant::~PowerPlant() noexcept {}

void PowerPlant::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 1 );
}
}
