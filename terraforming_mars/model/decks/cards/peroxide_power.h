#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class PeroxidePower : public AutomatedCard
{
public:
    PeroxidePower( const GameModel& model ) noexcept;
    ~PeroxidePower() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
