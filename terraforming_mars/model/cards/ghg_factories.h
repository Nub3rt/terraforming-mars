#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class GhgFactories : public AutomatedCard
{
public:
    GhgFactories( const GameModel& model ) noexcept;
    ~GhgFactories() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
