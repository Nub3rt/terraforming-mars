#pragma once

#include "card.h"
#include "card_id.h"

namespace model::decks
{
class ActiveCard : public Card
{
public:
    virtual ~ActiveCard() noexcept;

    bool IsActive() const noexcept override;

protected:
    ActiveCard( const GameModel& model, CardID card_id, int base_cost, bool is_building, bool is_space ) noexcept;
};
}
