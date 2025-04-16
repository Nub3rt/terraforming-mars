#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
