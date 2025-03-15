#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class UndergroundCity : public AutomatedCard
{
public:
    UndergroundCity( const GameModel& model ) noexcept;
    ~UndergroundCity() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
