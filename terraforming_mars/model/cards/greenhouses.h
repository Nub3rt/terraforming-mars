#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class Greenhouses : public AutomatedCard
{
public:
    Greenhouses( const GameModel& model ) noexcept;
    ~Greenhouses() noexcept;

protected:
    void ApplyImmediateEffects( const GameModel& _model ) override;
};
}
