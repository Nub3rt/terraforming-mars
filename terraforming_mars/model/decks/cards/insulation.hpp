#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class Insulation : public AutomatedCard
{
public:
    Insulation( const GameModel& model ) noexcept;
    ~Insulation() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
