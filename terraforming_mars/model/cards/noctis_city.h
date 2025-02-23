#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class NoctisCity : public AutomatedCard
{
public:
    NoctisCity() noexcept;
    ~NoctisCity() noexcept;

protected:
    bool SatisfiesRequirements( const GameModel& model ) const override;
    void ApplyImmediateEffects( const GameModel& model ) override;
};
}
