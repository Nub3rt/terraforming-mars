#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class FueledGenerators : public AutomatedCard
{
public:
    FueledGenerators( const GameModel& model ) noexcept;
    ~FueledGenerators() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
