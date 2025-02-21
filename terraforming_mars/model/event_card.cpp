#include "event_card.h"

#include "card.h"

namespace model::decks
{
EventCard::EventCard() noexcept : Card() {}
EventCard::~EventCard() noexcept {}

bool EventCard::IsEvent() const noexcept { return true; }
}
