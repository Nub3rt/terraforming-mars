#include "industrial_microbes.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
IndustrialMicrobes::IndustrialMicrobes( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::INDUSTRIAL_MICROBES, 12 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::MICROBE );
}

IndustrialMicrobes::~IndustrialMicrobes() noexcept {}

void IndustrialMicrobes::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::STEEL, 1 );
    _owner->GainResourceProduction( Resource::ENERGY, 1 );
}
}
