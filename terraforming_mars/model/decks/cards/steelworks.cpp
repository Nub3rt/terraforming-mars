#include "steelworks.hpp"

#include "../active_card_with_action.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
Steelworks::Steelworks( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::STEELWORKS, 15 ), _action_energy_cost( 4 ) {
    AddTag( Tag::BUILDING );
}

Steelworks::~Steelworks() noexcept {}

bool Steelworks::CanBeUsed() const {
    return _owner->GetResource( Resource::ENERGY ) >= _action_energy_cost;
}

void Steelworks::DoUseAction() {
    _owner->GainResource( Resource::STEEL, 2 );
    _owner->RaiseOxygen();
}
}
