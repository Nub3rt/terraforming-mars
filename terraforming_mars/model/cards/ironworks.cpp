#include "ironworks.h"

#include "../active_card_with_action.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
Ironworks::Ironworks( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::IRONWORKS, 11, true, false ), _action_energy_cost( 4 ) {
    AddTag( Tag::BUILDING );
}

Ironworks::~Ironworks() noexcept {}

bool Ironworks::CanBeUsed() const {
    return _owner->get_energy() >= _action_energy_cost;
}

void Ironworks::DoUseAction( const GameModel& _model ) {
    _owner->GainSteel( 1 );
    _owner->RaiseOxygen();
}
}
