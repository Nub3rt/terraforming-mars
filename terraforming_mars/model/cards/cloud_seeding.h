#pragma once

#include "../automated_card.h"
#include "../game_model.h"

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
