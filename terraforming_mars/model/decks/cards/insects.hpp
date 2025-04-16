#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class Insects : public AutomatedCard
{
public:
    Insects( const GameModel& model ) noexcept;
    ~Insects() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
