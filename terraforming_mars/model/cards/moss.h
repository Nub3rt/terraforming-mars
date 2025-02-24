#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class Moss : public AutomatedCard
{
public:
    Moss( const GameModel& model ) noexcept;
    ~Moss() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
