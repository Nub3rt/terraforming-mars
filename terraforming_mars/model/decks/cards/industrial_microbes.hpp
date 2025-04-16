#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class IndustrialMicrobes : public AutomatedCard
{
public:
    IndustrialMicrobes( const GameModel& model ) noexcept;
    ~IndustrialMicrobes() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
