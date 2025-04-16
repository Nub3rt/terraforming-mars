#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class NuclearPower : public AutomatedCard
{
public:
    NuclearPower( const GameModel& model ) noexcept;
    ~NuclearPower() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
