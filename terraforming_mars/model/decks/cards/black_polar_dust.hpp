#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class BlackPolarDust : public AutomatedCard
{
public:
    BlackPolarDust( const GameModel& model ) noexcept;
    ~BlackPolarDust() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
