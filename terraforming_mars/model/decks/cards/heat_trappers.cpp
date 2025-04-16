#include "heat_trappers.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
