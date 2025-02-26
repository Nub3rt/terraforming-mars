#pragma once

#include "card.h"
#include "card_id.h"

namespace model::decks
{
class AutomatedCard : public Card
{
public:
    virtual ~AutomatedCard() noexcept;

    bool IsAutomated() const noexcept override;

protected:
    AutomatedCard( const GameModel& model, CardID card_id, int base_cost ) noexcept;
};
}
