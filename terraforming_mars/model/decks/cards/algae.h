#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

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
