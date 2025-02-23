#include "event_card.h"

#include "card.h"
#include "card_id.h"

namespace model::decks
{
EventCard::EventCard( CardID card_id, int base_cost, bool is_building, bool is_space ) noexcept :
    Card( card_id, base_cost, is_building, is_space ) {
}

EventCard::~EventCard() noexcept {}

bool EventCard::IsEvent() const noexcept { return true; }
}
