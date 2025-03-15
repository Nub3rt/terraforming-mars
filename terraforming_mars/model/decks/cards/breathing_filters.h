#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

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
