#pragma once

#include "../automated_card.h"
#include "../game_model.h"

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
