#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

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
