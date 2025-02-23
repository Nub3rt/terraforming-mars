#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class ArtificalLake : public AutomatedCard
{
public:
    ArtificalLake() noexcept;
    ~ArtificalLake() noexcept;

protected:
    bool SatisfiesRequirements( const GameModel& _model ) const override;
    void ApplyImmediateEffects( const GameModel& _model ) override;
    int DoCountVPs( const GameModel& _model ) const override;
};
}
