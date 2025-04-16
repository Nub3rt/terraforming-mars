#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class FoodFactory : public AutomatedCard
{
public:
    FoodFactory( const GameModel& model ) noexcept;
    ~FoodFactory() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
