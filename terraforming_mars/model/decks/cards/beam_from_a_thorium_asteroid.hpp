#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
