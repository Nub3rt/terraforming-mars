#include "equatorial_magnetizer.h"

#include "../active_card_with_action.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
EquatorialMagnetizer::EquatorialMagnetizer( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::EQUATORIAL_MAGNETIZER, 11, true, false ), _action_energy_production_cost( 1 ) {
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
