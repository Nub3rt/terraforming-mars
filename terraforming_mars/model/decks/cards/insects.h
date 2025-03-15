#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

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
