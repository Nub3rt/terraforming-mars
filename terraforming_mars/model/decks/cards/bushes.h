#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class Bushes : public AutomatedCard
{
public:
    Bushes( const GameModel& model ) noexcept;
    ~Bushes() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
