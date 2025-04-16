#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
