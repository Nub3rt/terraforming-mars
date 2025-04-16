#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
