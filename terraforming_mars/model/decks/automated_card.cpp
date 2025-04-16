#include "automated_card.hpp"

#include "card.hpp"
#include "card_id.hpp"

namespace model::decks
{
AutomatedCard::AutomatedCard( const GameModel& model, CardID card_id, int base_cost ) noexcept :
    Card( model, card_id, base_cost ) {
}

AutomatedCard::~AutomatedCard() noexcept {}

bool AutomatedCard::IsAutomated() const noexcept { return true; }
}
