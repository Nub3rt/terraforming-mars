#include "optimal_aerobraking.h"

#include "../active_card_with_effect.h"
#include "../card_id.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
OptimalAerobraking::OptimalAerobraking() noexcept :
    ActiveCardWithEffect( CardID::OPTIMAL_AEROBRAKING, 7, false, true ) {
    AddTag( Tag::SPACE );
}

OptimalAerobraking::~OptimalAerobraking() noexcept {}

void OptimalAerobraking::DoAfterYouPlaySpaceEvent() {
    _owner->GainCredit( 3 );
    _owner->GainHeat( 3 );
}
}
