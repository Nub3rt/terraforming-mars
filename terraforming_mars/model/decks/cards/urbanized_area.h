#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class UrbanizedArea : public AutomatedCard
{
public:
    UrbanizedArea( const GameModel& model ) noexcept;
    ~UrbanizedArea() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
