#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
