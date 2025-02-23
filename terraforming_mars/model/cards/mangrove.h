#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class Mangrove : public AutomatedCard
{
public:
    Mangrove( const GameModel& model ) noexcept;
    ~Mangrove() noexcept;

protected:
    bool SatisfiesRequirements( const GameModel& _model ) const override;
    void ApplyImmediateEffects( const GameModel& _model ) override;
    int DoCountVPs( const GameModel& _model ) const override;
};
}
