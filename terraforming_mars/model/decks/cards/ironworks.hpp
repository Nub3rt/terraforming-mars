#pragma once

#include "../active_card_with_action.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class Ironworks : public ActiveCardWithAction
{
public:
    Ironworks( const GameModel& model ) noexcept;
    ~Ironworks() noexcept;

protected:
    int _action_energy_cost;

    bool CanBeUsed() const override;
    void DoUseAction() override;
};
}
