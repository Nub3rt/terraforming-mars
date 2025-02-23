#pragma once

#include "../active_card_with_action.h"
#include "../game_model.h"

namespace model::decks::cards
{
class OreProcessor : public ActiveCardWithAction
{
public:
    OreProcessor() noexcept;
    ~OreProcessor() noexcept;

protected:
    int _action_energy_cost;

    bool CanBeUsed() const override;
    void DoUseAction( const GameModel& model ) override;
};
}
