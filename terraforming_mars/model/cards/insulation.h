#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class Insulation : public AutomatedCard
{
public:
    Insulation() noexcept;
    ~Insulation() noexcept;

protected:
    void ApplyImmediateEffects( const GameModel& model ) override;
};
}
