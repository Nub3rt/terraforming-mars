#include "energy_saving.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
EnergySaving::EnergySaving() noexcept :
    AutomatedCard( CardID::ENERGY_SAVING, 15, false, false ) {
    AddTag( Tag::POWER );
}

EnergySaving::~EnergySaving() noexcept {}

void EnergySaving::ApplyImmediateEffects( const GameModel& model ) {
    _owner->GainEnergyProduction( model.CityCount() );
}
}
