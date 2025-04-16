#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
