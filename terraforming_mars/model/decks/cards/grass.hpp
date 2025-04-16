#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class Grass : public AutomatedCard
{
public:
    Grass( const GameModel& model ) noexcept;
    ~Grass() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
