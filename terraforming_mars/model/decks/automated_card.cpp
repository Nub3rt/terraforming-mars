#include "automated_card.h"

#include "card.h"
#include "card_id.h"

namespace model::decks
{
AutomatedCard::AutomatedCard( const GameModel& model, CardID card_id, int base_cost ) noexcept :
    Card( model, card_id, base_cost ) {
}

AutomatedCard::~AutomatedCard() noexcept {}

bool AutomatedCard::IsAutomated() const noexcept { return true; }
}
