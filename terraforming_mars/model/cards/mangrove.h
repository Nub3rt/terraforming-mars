#pragma once

#include "../automated_card.h"
#include "../game_model.h"

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
