#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class RadChemFactory : public AutomatedCard
{
public:
    RadChemFactory( const GameModel& model ) noexcept;
    ~RadChemFactory() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
