#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class ArtificalLake : public AutomatedCard
{
public:
    ArtificalLake( const GameModel& model ) noexcept;
    ~ArtificalLake() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
