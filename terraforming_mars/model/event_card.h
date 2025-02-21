#pragma once

#include "card.h"

namespace model::decks
{
class EventCard : public Card
{
public:
    virtual ~EventCard() noexcept;

    bool IsEvent() const noexcept override;

protected:
    EventCard() noexcept;
};
}
