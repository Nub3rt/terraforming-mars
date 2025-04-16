#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class Bushes : public AutomatedCard
{
public:
    Bushes( const GameModel& model ) noexcept;
    ~Bushes() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
