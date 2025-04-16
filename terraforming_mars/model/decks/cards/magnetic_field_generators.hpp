#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class MagneticFieldGenerators : public AutomatedCard
{
public:
    MagneticFieldGenerators( const GameModel& model ) noexcept;
    ~MagneticFieldGenerators() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
