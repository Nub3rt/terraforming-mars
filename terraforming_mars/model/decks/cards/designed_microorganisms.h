#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

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
