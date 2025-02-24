#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class OpenCity : public AutomatedCard
{
public:
    OpenCity( const GameModel& model ) noexcept;
    ~OpenCity() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
