#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class Moss : public AutomatedCard
{
public:
    Moss( const GameModel& model ) noexcept;
    ~Moss() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
