#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class Plantation : public AutomatedCard
{
public:
    Plantation( const GameModel& model ) noexcept;
    ~Plantation() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
