#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class CupolaCity : public AutomatedCard
{
public:
    CupolaCity( const GameModel& model ) noexcept;
    ~CupolaCity() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
