#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class Algae : public AutomatedCard
{
public:
    Algae( const GameModel& model ) noexcept;
    ~Algae() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
