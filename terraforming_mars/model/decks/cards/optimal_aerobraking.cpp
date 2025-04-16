#include "optimal_aerobraking.hpp"

#include "../active_card_with_effect.hpp"
#include "../card_id.hpp"
#include "../../player.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
OptimalAerobraking::OptimalAerobraking( const GameModel& model ) noexcept :
    ActiveCardWithEffect( model, CardID::OPTIMAL_AEROBRAKING, 7 ) {
    AddTag( Tag::SPACE );
}

OptimalAerobraking::~OptimalAerobraking() noexcept {}

void OptimalAerobraking::DoAfterYouPlaySpaceEvent() {
    _owner->GainResource( Resource::CREDIT, 3 );
    _owner->GainResource( Resource::HEAT, 3 );
}
}
