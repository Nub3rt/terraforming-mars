#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class Mangrove : public AutomatedCard
{
public:
    Mangrove( const GameModel& model ) noexcept;
    ~Mangrove() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
