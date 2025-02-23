#pragma once

#include "../active_card_with_action.h"
#include "../game_model.h"

namespace model::decks::cards
{
class SpaceMirrors : public ActiveCardWithAction
{
public:
    SpaceMirrors() noexcept;
    ~SpaceMirrors() noexcept;

protected:
    int _action_credit_cost;

    bool CanBeUsed() const override;
    void DoUseAction( const GameModel& model ) override;
};
}
