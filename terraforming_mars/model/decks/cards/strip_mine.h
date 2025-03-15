#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class StripMine : public AutomatedCard
{
public:
    StripMine( const GameModel& model ) noexcept;
    ~StripMine() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
