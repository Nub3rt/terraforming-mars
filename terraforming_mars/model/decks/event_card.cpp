#include "event_card.hpp"

#include "card.hpp"
#include "card_id.hpp"

namespace model::decks
{
EventCard::EventCard( const GameModel& model, CardID card_id, int base_cost ) noexcept :
    Card( model, card_id, base_cost ) {
}

EventCard::~EventCard() noexcept {}

bool EventCard::IsEvent() const noexcept { return true; }
}
