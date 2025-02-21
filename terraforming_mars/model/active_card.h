#pragma once

#include "card.h"

namespace model::decks
{
class ActiveCard : public Card
{
public:
    virtual ~ActiveCard() noexcept;

    bool IsActive() const noexcept override;

protected:
    ActiveCard() noexcept;
};
}
