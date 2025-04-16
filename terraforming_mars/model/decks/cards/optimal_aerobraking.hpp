#pragma once

#include "../active_card_with_effect.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class OptimalAerobraking : public ActiveCardWithEffect
{
public:
    OptimalAerobraking( const GameModel& model ) noexcept;
    ~OptimalAerobraking() noexcept;

protected:
    void DoAfterYouPlaySpaceEvent() override;
};
}
