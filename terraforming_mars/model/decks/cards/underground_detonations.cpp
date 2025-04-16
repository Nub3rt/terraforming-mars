#include "underground_detonations.hpp"

#include "../active_card_with_action.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
UndergroundDetonations::UndergroundDetonations( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::UNDERGROUND_DETONATIONS, 6 ), _action_credit_cost( 10 ) {
    AddTag( Tag::BUILDING );
}

UndergroundDetonations::~UndergroundDetonations() noexcept {}

bool UndergroundDetonations::CanBeUsed() const {
    return _owner->GetResource( Resource::CREDIT ) >= _action_credit_cost;
}

void UndergroundDetonations::DoUseAction() {
    _owner->LoseResource( Resource::CREDIT, _action_credit_cost );

    _owner->GainResourceProduction( Resource::HEAT, 2 );
}
}
