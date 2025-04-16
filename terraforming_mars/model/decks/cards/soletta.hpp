#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class Soletta : public AutomatedCard
{
public:
    Soletta( const GameModel& model ) noexcept;
    ~Soletta() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
