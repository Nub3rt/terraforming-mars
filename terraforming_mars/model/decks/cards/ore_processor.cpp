#include "ore_processor.hpp"

#include "../active_card_with_action.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
