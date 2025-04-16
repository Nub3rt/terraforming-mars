#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
