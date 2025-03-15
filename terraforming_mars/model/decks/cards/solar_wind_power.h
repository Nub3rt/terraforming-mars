#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class SolarWindPower : public AutomatedCard
{
public:
    SolarWindPower( const GameModel& model ) noexcept;
    ~SolarWindPower() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
