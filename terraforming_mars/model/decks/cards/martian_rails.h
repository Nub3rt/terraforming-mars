#pragma once

#include "../active_card_with_action.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class MartianRails : public ActiveCardWithAction
{
public:
    MartianRails( const GameModel& model ) noexcept;
    ~MartianRails() noexcept;

protected:
    int _action_energy_cost;

    bool CanBeUsed() const override;
    void DoUseAction() override;
};
}
