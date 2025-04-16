#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
