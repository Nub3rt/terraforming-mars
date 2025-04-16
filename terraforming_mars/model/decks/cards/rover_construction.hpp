#pragma once

#include "../active_card_with_effect.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class RoverConstruction : public ActiveCardWithEffect
{
public:
    RoverConstruction( const GameModel& model ) noexcept;
    ~RoverConstruction() noexcept;

protected:
    int DoCountVPs() const override;

    void DoAfterAnyonePlacesCity() override;
};
}
