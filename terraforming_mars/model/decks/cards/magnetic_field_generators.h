#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

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
