#include "rover_construction.h"

#include "../active_card_with_effect.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
RoverConstruction::RoverConstruction( const GameModel& model ) noexcept :
    ActiveCardWithEffect( model, CardID::ROVER_CONSTRUCTION, 8 ) {
    AddTag( Tag::BUILDING );
}

RoverConstruction::~RoverConstruction() noexcept {}

int RoverConstruction::DoCountVPs() const { return 1; }

void RoverConstruction::DoAfterAnyonePlacesCity() {
    _owner->GainResource( Resource::CREDIT, 2 );
}
}
