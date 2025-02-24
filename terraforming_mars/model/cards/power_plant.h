#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class PowerPlant : public AutomatedCard
{
public:
    PowerPlant( const GameModel& model ) noexcept;
    ~PowerPlant() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
