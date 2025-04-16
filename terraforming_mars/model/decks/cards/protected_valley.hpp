#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class ProtectedValley : public AutomatedCard
{
public:
    ProtectedValley( const GameModel& model ) noexcept;
    ~ProtectedValley() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
