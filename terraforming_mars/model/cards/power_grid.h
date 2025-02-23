#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class PowerGrid : public AutomatedCard
{
public:
    PowerGrid() noexcept;
    ~PowerGrid() noexcept;

protected:
    void ApplyImmediateEffects( const GameModel& model ) override;
};
}
