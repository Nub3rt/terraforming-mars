#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class GeothermalPower : public AutomatedCard
{
public:
    GeothermalPower( const GameModel& model ) noexcept;
    ~GeothermalPower() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
