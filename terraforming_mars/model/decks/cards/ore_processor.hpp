#pragma once

#include "../active_card_with_action.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class OreProcessor : public ActiveCardWithAction
{
public:
    OreProcessor( const GameModel& model ) noexcept;
    ~OreProcessor() noexcept;

protected:
    int _action_energy_cost;

    bool CanBeUsed() const override;
    void DoUseAction() override;
};
}
