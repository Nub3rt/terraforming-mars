#include "rover_construction.hpp"

#include "../active_card_with_effect.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
