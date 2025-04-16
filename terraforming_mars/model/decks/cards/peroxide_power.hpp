#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
