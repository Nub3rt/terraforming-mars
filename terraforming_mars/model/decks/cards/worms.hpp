#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
