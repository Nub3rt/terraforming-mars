#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class Worms : public AutomatedCard
{
public:
    Worms( const GameModel& model ) noexcept;
    ~Worms() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
