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
    void UseAction( const GameModel& model );
    inline void NextGenerationStarted() noexcept;

protected:
    ActiveCardWithAction( CardID card_id, int base_cost, bool is_building, bool is_space ) noexcept;

    bool _used_this_generation;

    virtual bool CanBeUsed() const = 0;
    virtual void DoUseAction( const GameModel& model ) = 0;
};
}
