#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
