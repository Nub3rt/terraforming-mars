#pragma once

#include "card.hpp"
#include "card_id.hpp"

namespace model::decks
{
class EventCard : public Card
{
public:
    virtual ~EventCard() noexcept;

    bool IsEvent() const noexcept override;

protected:
    EventCard( const GameModel& model, CardID card_id, int base_cost ) noexcept;
};
}
