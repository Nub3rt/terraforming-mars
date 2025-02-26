#include "energy_saving.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
EnergySaving::EnergySaving( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::ENERGY_SAVING, 15 ) {
    AddTag( Tag::POWER );
}

EnergySaving::~EnergySaving() noexcept {}

void EnergySaving::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, _model.CityCount() );
}
}
