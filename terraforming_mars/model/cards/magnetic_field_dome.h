#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class MagneticFieldDome : public AutomatedCard
{
public:
    MagneticFieldDome( const GameModel& model ) noexcept;
    ~MagneticFieldDome() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
