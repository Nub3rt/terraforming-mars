#include "underground_detonations.h"

#include "../active_card_with_action.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
UndergroundDetonations::UndergroundDetonations( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::UNDERGROUND_DETONATIONS, 6, true, false ), _action_credit_cost( 10 ) {
    AddTag( Tag::BUILDING );
}

UndergroundDetonations::~UndergroundDetonations() noexcept {}

bool UndergroundDetonations::CanBeUsed() const {
    return _owner->get_credit() >= _action_credit_cost;
}

void UndergroundDetonations::DoUseAction() {
    _owner->LoseCredit( _action_credit_cost );

    _owner->GainHeatProduction( 2 );
}
}
