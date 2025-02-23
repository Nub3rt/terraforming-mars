#include "underground_detonations.h"

#include "../active_card_with_action.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
UndergroundDetonations::UndergroundDetonations() noexcept :
    ActiveCardWithAction( CardID::UNDERGROUND_DETONATIONS, 6, true, false ), _action_credit_cost( 10 ) {
    AddTag( Tag::BUILDING );
}

UndergroundDetonations::~UndergroundDetonations() noexcept {}

bool UndergroundDetonations::CanBeUsed() const {
    return _owner->get_credit() >= _action_credit_cost;
}

void UndergroundDetonations::DoUseAction( const GameModel& _model ) {
    _owner->LoseCredit( _action_credit_cost );

    _owner->GainHeatProduction( 2 );
}
}
