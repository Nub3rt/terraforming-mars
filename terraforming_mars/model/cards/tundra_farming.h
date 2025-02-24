#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class TundraFarming : public AutomatedCard
{
public:
    TundraFarming( const GameModel& model ) noexcept;
    ~TundraFarming() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
