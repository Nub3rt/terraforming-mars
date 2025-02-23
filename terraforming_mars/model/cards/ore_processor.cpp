#include "ore_processor.h"

#include "../active_card_with_action.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
OreProcessor::OreProcessor( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::ORE_PROCESSOR, 13, true, false ), _action_energy_cost( 4 ) {
    AddTag( Tag::BUILDING );
}

OreProcessor::~OreProcessor() noexcept {}

bool OreProcessor::CanBeUsed() const {
    return _owner->get_energy() >= _action_energy_cost;
}

void OreProcessor::DoUseAction( const GameModel& _model ) {
    _owner->GainTitanium( 1 );
    _owner->RaiseOxygen();
}
}
