#pragma once

#include "../active_card_with_action.h"
#include "../game_model.h"

namespace model::decks::cards
{
class WaterImportFromEuropa : public ActiveCardWithAction
{
public:
    WaterImportFromEuropa( const GameModel& model ) noexcept;
    ~WaterImportFromEuropa() noexcept;

protected:
    int _action_credit_cost;

    int DoCountVPs() const override;

    bool CanBeUsed() const override;
    void DoUseAction() override;
};
}
