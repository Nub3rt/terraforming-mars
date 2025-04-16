#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
