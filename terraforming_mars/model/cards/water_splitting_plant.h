#pragma once

#include "../active_card_with_action.h"
#include "../game_model.h"

namespace model::decks::cards
{
class WaterSplittingPlant : public ActiveCardWithAction
{
public:
    WaterSplittingPlant() noexcept;
    ~WaterSplittingPlant() noexcept;

protected:
    int _action_energy_cost;

    bool SatisfiesRequirements( const GameModel& _model ) const override;

    bool CanBeUsed() const override;
    void DoUseAction( const GameModel& _model ) override;
};
}
