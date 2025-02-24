#pragma once

#include "../automated_card.h"
#include "../game_model.h"

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
