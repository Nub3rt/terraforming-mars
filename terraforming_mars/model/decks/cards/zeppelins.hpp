#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
