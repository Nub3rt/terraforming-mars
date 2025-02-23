#include "martian_rails.h"

#include "../active_card_with_action.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
MartianRails::MartianRails( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::MARTIAN_RAILS, 13, true, false ), _action_energy_cost( 1 ) {
    AddTag( Tag::BUILDING );
}

MartianRails::~MartianRails() noexcept {}

bool MartianRails::CanBeUsed() const {
    return _owner->get_energy() >= _action_energy_cost;
}

void MartianRails::DoUseAction( const GameModel& _model ) {
    _owner->LoseEnergy( _action_energy_cost );

    _owner->GainCredit( _model.CityCount() );
}
}
