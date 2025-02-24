#pragma once

#include "../automated_card.h"
#include "../game_model.h"

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
