#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class GreatDam : public AutomatedCard
{
public:
    GreatDam( const GameModel& model ) noexcept;
    ~GreatDam() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
