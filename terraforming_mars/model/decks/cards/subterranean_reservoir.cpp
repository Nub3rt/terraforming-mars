#include "subterranean_reservoir.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
