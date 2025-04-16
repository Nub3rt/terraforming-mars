#include "energy_saving.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
