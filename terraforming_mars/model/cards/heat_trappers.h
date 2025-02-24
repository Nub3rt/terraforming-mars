#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class HeatTrappers : public AutomatedCard
{
public:
    HeatTrappers( const GameModel& model ) noexcept;
    ~HeatTrappers() noexcept;

protected:
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
