#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class ColonizerTrainingCamp : public AutomatedCard
{
public:
    ColonizerTrainingCamp( const GameModel& model ) noexcept;
    ~ColonizerTrainingCamp() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
