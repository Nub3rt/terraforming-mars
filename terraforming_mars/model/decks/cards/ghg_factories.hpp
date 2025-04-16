#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
