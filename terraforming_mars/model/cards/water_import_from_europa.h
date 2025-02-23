#pragma once

#include "../active_card_with_action.h"
#include "../game_model.h"

namespace model::decks::cards
{
class WaterImportFromEuropa : public ActiveCardWithAction
{
public:
    WaterImportFromEuropa() noexcept;
    ~WaterImportFromEuropa() noexcept;

protected:
    int _action_credit_cost;

    int DoCountVPs( const GameModel& _model ) const override;

    bool CanBeUsed() const override;
    void DoUseAction( const GameModel& _model ) override;
};
}
