#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class BeamFromAThoriumAsteroid : public AutomatedCard
{
public:
    BeamFromAThoriumAsteroid( const GameModel& model ) noexcept;
    ~BeamFromAThoriumAsteroid() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
