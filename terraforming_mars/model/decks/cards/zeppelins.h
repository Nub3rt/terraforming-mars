#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class Zeppelins : public AutomatedCard
{
public:
    Zeppelins( const GameModel& model ) noexcept;
    ~Zeppelins() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
