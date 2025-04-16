#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
