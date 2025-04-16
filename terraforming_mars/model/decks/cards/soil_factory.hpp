#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class SoilFactory : public AutomatedCard
{
public:
    SoilFactory( const GameModel& model ) noexcept;
    ~SoilFactory() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
