#pragma once

#include "../active_card_with_action.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class Steelworks : public ActiveCardWithAction
{
public:
    Steelworks( const GameModel& model ) noexcept;
    ~Steelworks() noexcept;

protected:
    int _action_energy_cost;

    bool CanBeUsed() const override;
    void DoUseAction() override;
};
}
