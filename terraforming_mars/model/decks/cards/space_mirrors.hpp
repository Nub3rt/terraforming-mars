#pragma once

#include "../active_card_with_action.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class SpaceMirrors : public ActiveCardWithAction
{
public:
    SpaceMirrors( const GameModel& model ) noexcept;
    ~SpaceMirrors() noexcept;

protected:
    int _action_credit_cost;

    bool CanBeUsed() const override;
    void DoUseAction() override;
};
}
