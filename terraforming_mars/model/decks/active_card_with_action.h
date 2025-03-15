#pragma once

#include "active_card.h"
#include "availability.h"
#include "card_id.h"

namespace model::decks
{
class ActiveCardWithAction : public ActiveCard
{
public:
    virtual ~ActiveCardWithAction() noexcept;

    bool IsActiveWithAction() const noexcept override;

    Availability Availability() const;
    void UseAction();
    inline void NextGenerationStarted() noexcept;

protected:
    ActiveCardWithAction( const GameModel& model, CardID card_id, int base_cost ) noexcept;

    bool _used_this_generation;

    virtual bool CanBeUsed() const = 0;
    virtual void DoUseAction() = 0;
};
}
