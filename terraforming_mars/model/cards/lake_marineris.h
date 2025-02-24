#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class LakeMarineris : public AutomatedCard
{
public:
    LakeMarineris( const GameModel& model ) noexcept;
    ~LakeMarineris() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
