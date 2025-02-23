#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class NoctisCity : public AutomatedCard
{
public:
    NoctisCity( const GameModel& model ) noexcept;
    ~NoctisCity() noexcept;

protected:
    bool SatisfiesRequirements( const GameModel& _model ) const override;
    void ApplyImmediateEffects( const GameModel& _model ) override;
};
}
