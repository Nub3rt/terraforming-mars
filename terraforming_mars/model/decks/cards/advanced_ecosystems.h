#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class AdvancedEcosystems : public AutomatedCard
{
public:
    AdvancedEcosystems( const GameModel& model ) noexcept;
    ~AdvancedEcosystems() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
