#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class KelpFarming : public AutomatedCard
{
public:
    KelpFarming( const GameModel& model ) noexcept;
    ~KelpFarming() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
