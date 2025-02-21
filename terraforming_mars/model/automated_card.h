#pragma once

#include "card.h"

namespace model::decks
{
class AutomatedCard : public Card
{
public:
    virtual ~AutomatedCard() noexcept;

    bool IsAutomated() const noexcept override;

protected:
    AutomatedCard() noexcept;
};
}
