#include "equatorial_magnetizer.hpp"

#include "../active_card_with_action.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
EquatorialMagnetizer::EquatorialMagnetizer( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::EQUATORIAL_MAGNETIZER, 11 ), _action_energy_production_cost( 1 ) {
    AddTag( Tag::BUILDING );
}

EquatorialMagnetizer::~EquatorialMagnetizer() noexcept {}

bool EquatorialMagnetizer::CanBeUsed() const {
    return _owner->GetResourceProduction( Resource::ENERGY ) >= _action_energy_production_cost;
}

void EquatorialMagnetizer::DoUseAction() {
    _owner->LoseResourceProduction( Resource::ENERGY, _action_energy_production_cost );

    _owner->RaiseTR( 1 );
}
}
