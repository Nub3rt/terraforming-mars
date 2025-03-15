#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

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
