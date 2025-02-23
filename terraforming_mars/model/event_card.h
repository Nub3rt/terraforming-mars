#pragma once

#include "card.h"
#include "card_id.h"

namespace model::decks
{
class EventCard : public Card
{
public:
    virtual ~EventCard() noexcept;

    bool IsEvent() const noexcept override;

protected:
    EventCard( const GameModel& model, CardID card_id, int base_cost, bool is_building, bool is_space ) noexcept;
};
}
