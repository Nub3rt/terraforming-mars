#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class Worms : public AutomatedCard
{
public:
    Worms() noexcept;
    ~Worms() noexcept;

protected:
    bool SatisfiesRequirements( const GameModel& _model ) const override;
    void ApplyImmediateEffects( const GameModel& _model ) override;
};
}
