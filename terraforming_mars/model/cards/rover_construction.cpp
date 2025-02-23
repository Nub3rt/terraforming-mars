#include "rover_construction.h"

#include "../active_card_with_effect.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
RoverConstruction::RoverConstruction() noexcept :
    ActiveCardWithEffect( CardID::ROVER_CONSTRUCTION, 8, true, false ) {
    AddTag( Tag::BUILDING );
}

RoverConstruction::~RoverConstruction() noexcept {}

int RoverConstruction::DoCountVPs( const GameModel& _model ) const { return 1; }

void RoverConstruction::DoAfterAnyonePlacesCity() {
    _owner->GainCredit( 2 );
}
}
