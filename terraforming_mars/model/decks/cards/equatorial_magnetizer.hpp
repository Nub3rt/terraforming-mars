#pragma once

#include "../active_card_with_action.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class EquatorialMagnetizer : public ActiveCardWithAction
{
public:
    EquatorialMagnetizer( const GameModel& model ) noexcept;
    ~EquatorialMagnetizer() noexcept;

protected:
    int _action_energy_production_cost;

    bool CanBeUsed() const override;
    void DoUseAction() override;
};
}
