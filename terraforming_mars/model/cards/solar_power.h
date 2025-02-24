#pragma once

#include "../automated_card.h"
#include "../game_model.h"

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
