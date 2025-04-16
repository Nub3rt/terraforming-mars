#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class BreathingFilters : public AutomatedCard
{
public:
    BreathingFilters( const GameModel& model ) noexcept;
    ~BreathingFilters() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
