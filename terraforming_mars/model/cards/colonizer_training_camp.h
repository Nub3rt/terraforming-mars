#pragma once

#include "../automated_card.h"
#include "../game_model.h"

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
