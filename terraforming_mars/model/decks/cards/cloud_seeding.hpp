#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class CloudSeeding : public AutomatedCard
{
public:
    CloudSeeding( const GameModel& model ) noexcept;
    ~CloudSeeding() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
