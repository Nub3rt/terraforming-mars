#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
