#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class DomedCrater : public AutomatedCard
{
public:
    DomedCrater( const GameModel& model ) noexcept;
    ~DomedCrater() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
