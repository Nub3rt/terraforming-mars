#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
