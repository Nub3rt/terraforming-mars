#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class Insulation : public AutomatedCard
{
public:
    Insulation( const GameModel& model ) noexcept;
    ~Insulation() noexcept;

protected:
    void ApplyImmediateEffects( const GameModel& _model ) override;
};
}
