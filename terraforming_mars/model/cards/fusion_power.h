#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class FusionPower : public AutomatedCard
{
public:
    FusionPower( const GameModel& model ) noexcept;
    ~FusionPower() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
