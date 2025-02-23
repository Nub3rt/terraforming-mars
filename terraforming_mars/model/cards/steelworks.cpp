#include "steelworks.h"

#include "../active_card_with_action.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
Steelworks::Steelworks( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::STEELWORKS, 15, true, false ), _action_energy_cost( 4 ) {
    AddTag( Tag::BUILDING );
}

Steelworks::~Steelworks() noexcept {}

bool Steelworks::CanBeUsed() const {
    return _owner->get_energy() >= _action_energy_cost;
}

void Steelworks::DoUseAction() {
    _owner->GainSteel( 2 );
    _owner->RaiseOxygen();
}
}
