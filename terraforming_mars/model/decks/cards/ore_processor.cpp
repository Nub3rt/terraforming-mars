#include "ore_processor.h"

#include "../active_card_with_action.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
OreProcessor::OreProcessor( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::ORE_PROCESSOR, 13 ), _action_energy_cost( 4 ) {
    AddTag( Tag::BUILDING );
}

OreProcessor::~OreProcessor() noexcept {}

bool OreProcessor::CanBeUsed() const {
    return _owner->GetResource( Resource::ENERGY ) >= _action_energy_cost;
}

void OreProcessor::DoUseAction() {
    _owner->GainResource( Resource::TITANIUM, 1 );
    _owner->RaiseOxygen();
}
}
