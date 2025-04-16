#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
