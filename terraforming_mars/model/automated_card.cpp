#include "automated_card.h"

#include "card.h"

namespace model::decks
{
AutomatedCard::AutomatedCard() noexcept : Card() {}
AutomatedCard::~AutomatedCard() noexcept {}

bool AutomatedCard::IsAutomated() const noexcept { return true; }
}
