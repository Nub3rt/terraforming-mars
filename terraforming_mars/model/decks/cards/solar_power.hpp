#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class SolarPower : public AutomatedCard
{
public:
    SolarPower( const GameModel& model ) noexcept;
    ~SolarPower() noexcept;

protected:
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
