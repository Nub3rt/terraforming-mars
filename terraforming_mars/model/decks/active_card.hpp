#pragma once

#include "card.hpp"
#include "card_id.hpp"

namespace model::decks
{
class ActiveCard : public Card
{
public:
    virtual ~ActiveCard() noexcept;

    bool IsActive() const noexcept override;

protected:
    ActiveCard( const GameModel& model, CardID card_id, int base_cost ) noexcept;
};
}
