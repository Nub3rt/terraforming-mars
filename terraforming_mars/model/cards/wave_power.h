#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class WavePower : public AutomatedCard
{
public:
    WavePower( const GameModel& model ) noexcept;
    ~WavePower() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
