#pragma once

#include "../active_card_with_effect.h"
#include "../game_model.h"

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
