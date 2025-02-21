#include "active_card.h"

#include "card.h"

namespace model::decks
{
ActiveCard::ActiveCard() noexcept : Card() {}
ActiveCard::~ActiveCard() noexcept {}

bool ActiveCard::IsActive() const noexcept { return true; }
}
