#pragma once

#include "../active_card_with_action.h"
#include "../game_model.h"

namespace model::decks::cards
{
class AquiferPumping : public ActiveCardWithAction
{
public:
    AquiferPumping() noexcept;
    ~AquiferPumping() noexcept;

protected:
    int _action_credit_cost;

    bool CanBeUsed() const override;
    void DoUseAction( const GameModel& _model ) override;
};
}
