#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class DesignedMicroorganisms : public AutomatedCard
{
public:
    DesignedMicroorganisms( const GameModel& model ) noexcept;
    ~DesignedMicroorganisms() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
