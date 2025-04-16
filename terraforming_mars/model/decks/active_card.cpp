#include "active_card.hpp"

#include "card.hpp"
#include "card_id.hpp"

namespace model::decks
{
ActiveCard::ActiveCard( const GameModel& model, CardID card_id, int base_cost ) noexcept :
    Card( model, card_id, base_cost ) {
}

ActiveCard::~ActiveCard() noexcept {}

bool ActiveCard::IsActive() const noexcept { return true; }
}
