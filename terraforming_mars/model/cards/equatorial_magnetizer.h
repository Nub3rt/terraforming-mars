#pragma once

#include "../active_card_with_action.h"
#include "../game_model.h"

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
    void DoUseAction( const GameModel& _model ) override;
};
}
