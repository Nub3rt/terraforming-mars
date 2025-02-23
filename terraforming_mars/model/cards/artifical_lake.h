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
    bool SatisfiesRequirements( const GameModel& model ) const override;
    void ApplyImmediateEffects( const GameModel& model ) override;
    int DoCountVPs( const GameModel& model ) const override;
};
}
