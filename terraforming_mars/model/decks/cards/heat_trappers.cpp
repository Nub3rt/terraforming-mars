#include "heat_trappers.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
HeatTrappers::HeatTrappers( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::HEAT_TRAPPERS, 6 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

HeatTrappers::~HeatTrappers() noexcept {}

void HeatTrappers::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 1 );
    _owner->DestroyResourceProduction( Resource::HEAT, 2 );
}

int HeatTrappers::DoCountVPs() const {
    return -1;
}
}
