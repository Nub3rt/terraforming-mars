#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class CarbonateProcessing : public AutomatedCard
{
public:
    CarbonateProcessing( const GameModel& model ) noexcept;
    ~CarbonateProcessing() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
