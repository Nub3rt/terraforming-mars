#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
