#pragma once

#include "../active_card_with_action.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class AquiferPumping : public ActiveCardWithAction
{
public:
    AquiferPumping( const GameModel& model ) noexcept;
    ~AquiferPumping() noexcept;

protected:
    int _action_credit_cost;

    bool CanBeUsed() const override;
    void DoUseAction() override;
};
}
