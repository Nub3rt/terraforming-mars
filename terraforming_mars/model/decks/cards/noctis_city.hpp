#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class NoctisCity : public AutomatedCard
{
public:
    NoctisCity( const GameModel& model ) noexcept;
    ~NoctisCity() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
