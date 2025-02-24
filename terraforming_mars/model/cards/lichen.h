#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class Lichen : public AutomatedCard
{
public:
    Lichen( const GameModel& model ) noexcept;
    ~Lichen() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
