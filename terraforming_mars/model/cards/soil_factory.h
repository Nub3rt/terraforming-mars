#pragma once

#include "../automated_card.h"
#include "../game_model.h"

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
