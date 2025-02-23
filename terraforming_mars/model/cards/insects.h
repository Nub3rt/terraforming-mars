#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class Insects : public AutomatedCard
{
public:
    Insects() noexcept;
    ~Insects() noexcept;

protected:
    bool SatisfiesRequirements( const GameModel& _model ) const override;
    void ApplyImmediateEffects( const GameModel& _model ) override;
};
}
