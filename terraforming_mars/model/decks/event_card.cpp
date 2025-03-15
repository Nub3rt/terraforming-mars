#include "event_card.h"

#include "card.h"
#include "card_id.h"

namespace model::decks
{
EventCard::EventCard( const GameModel& model, CardID card_id, int base_cost ) noexcept :
    Card( model, card_id, base_cost ) {
}

EventCard::~EventCard() noexcept {}

bool EventCard::IsEvent() const noexcept { return true; }
}
