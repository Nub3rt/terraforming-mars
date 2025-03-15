#include "subterranean_reservoir.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
SubterraneanReservoir::SubterraneanReservoir( const GameModel& model ) noexcept :
    EventCard( model, CardID::SUBTERRANEAN_RESERVOIR, 11 ) {
    AddTag( Tag::EVENT );
}

SubterraneanReservoir::~SubterraneanReservoir() noexcept {}

void SubterraneanReservoir::ApplyImmediateEffects() {
    _owner->PlaceOcean();
}
}
