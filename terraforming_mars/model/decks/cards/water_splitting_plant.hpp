#pragma once

#include "../active_card_with_action.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class WaterSplittingPlant : public ActiveCardWithAction
{
public:
    WaterSplittingPlant( const GameModel& model ) noexcept;
    ~WaterSplittingPlant() noexcept;

protected:
    int _action_energy_cost;

    bool SatisfiesRequirements() const override;

    bool CanBeUsed() const override;
    void DoUseAction() override;
};
}
