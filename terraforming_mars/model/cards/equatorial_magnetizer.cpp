#include "equatorial_magnetizer.h"

#include "../active_card_with_action.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
EquatorialMagnetizer::EquatorialMagnetizer() noexcept :
    ActiveCardWithAction( CardID::EQUATORIAL_MAGNETIZER, 11, true, false ), _action_energy_production_cost( 1 ) {
    AddTag( Tag::BUILDING );
}

EquatorialMagnetizer::~EquatorialMagnetizer() noexcept {}

bool EquatorialMagnetizer::CanBeUsed() const {
    return _owner->get_energy_production() >= _action_energy_production_cost;
}

void EquatorialMagnetizer::DoUseAction( const GameModel& model ) {
    _owner->LoseEnergyProduction( _action_energy_production_cost );

    _owner->RaiseTR( 1 );
}
}
