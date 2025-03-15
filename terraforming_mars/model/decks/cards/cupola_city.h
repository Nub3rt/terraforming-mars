#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class CupolaCity : public AutomatedCard
{
public:
    CupolaCity( const GameModel& model ) noexcept;
    ~CupolaCity() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
