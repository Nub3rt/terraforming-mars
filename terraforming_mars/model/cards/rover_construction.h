#pragma once

#include "../active_card_with_effect.h"
#include "../game_model.h"

namespace model::decks::cards
{
class RoverConstruction : public ActiveCardWithEffect
{
public:
    RoverConstruction( const GameModel& model ) noexcept;
    ~RoverConstruction() noexcept;

protected:
    int DoCountVPs( const GameModel& _model ) const override;

    void DoAfterAnyonePlacesCity() override;
};
}
