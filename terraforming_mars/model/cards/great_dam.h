#pragma once

#include "../automated_card.h"
#include "../game_model.h"

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
