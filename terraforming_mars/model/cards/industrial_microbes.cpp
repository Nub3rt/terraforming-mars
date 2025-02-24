#include "industrial_microbes.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
IndustrialMicrobes::IndustrialMicrobes( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::INDUSTRIAL_MICROBES, 12, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::MICROBE );
}

IndustrialMicrobes::~IndustrialMicrobes() noexcept {}

void IndustrialMicrobes::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::STEEL, 1 );
    _owner->GainResourceProduction( Resource::ENERGY, 1 );
}
}
