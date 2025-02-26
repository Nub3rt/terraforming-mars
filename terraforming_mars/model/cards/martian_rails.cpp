#include "martian_rails.h"

#include "../active_card_with_action.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
MartianRails::MartianRails( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::MARTIAN_RAILS, 13 ), _action_energy_cost( 1 ) {
    AddTag( Tag::BUILDING );
}

MartianRails::~MartianRails() noexcept {}

bool MartianRails::CanBeUsed() const {
    return _owner->GetResource( Resource::ENERGY ) >= _action_energy_cost;
}

void MartianRails::DoUseAction() {
    _owner->LoseResource( Resource::ENERGY, _action_energy_cost );

    _owner->GainResource( Resource::CREDIT, _model.CityCount() );
}
}
