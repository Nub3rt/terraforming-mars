#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class LunarBeam : public AutomatedCard
{
public:
    LunarBeam( const GameModel& model ) noexcept;
    ~LunarBeam() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
