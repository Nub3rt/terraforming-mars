#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class UrbanizedArea : public AutomatedCard
{
public:
    UrbanizedArea() noexcept;
    ~UrbanizedArea() noexcept;

protected:
    bool SatisfiesRequirements( const GameModel& model ) const override;
    void ApplyImmediateEffects( const GameModel& model ) override;
};
}
