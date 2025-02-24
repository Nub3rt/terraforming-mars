#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class Heather : public AutomatedCard
{
public:
    Heather( const GameModel& model ) noexcept;
    ~Heather() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
