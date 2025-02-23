#include "active_card.h"

#include "card.h"
#include "card_id.h"

namespace model::decks
{
ActiveCard::ActiveCard( CardID card_id, int base_cost, bool is_building, bool is_space ) noexcept :
    Card( card_id, base_cost, is_building, is_space ) {
}

ActiveCard::~ActiveCard() noexcept {}

bool ActiveCard::IsActive() const noexcept { return true; }
}
