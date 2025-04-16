#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class PowerGrid : public AutomatedCard
{
public:
    PowerGrid( const GameModel& model ) noexcept;
    ~PowerGrid() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
