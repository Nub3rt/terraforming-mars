#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
