#include "ironworks.hpp"

#include "../active_card_with_action.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
Ironworks::Ironworks( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::IRONWORKS, 11 ), _action_energy_cost( 4 ) {
    AddTag( Tag::BUILDING );
}

Ironworks::~Ironworks() noexcept {}

bool Ironworks::CanBeUsed() const {
    return _owner->GetResource( Resource::ENERGY ) >= _action_energy_cost;
}

void Ironworks::DoUseAction() {
    _owner->LoseResource( Resource::ENERGY, _action_energy_cost );

    _owner->GainResource( Resource::STEEL, 1 );
    _owner->RaiseOxygen();
}
}
