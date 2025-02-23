#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class ProtectedValley : public AutomatedCard
{
public:
    ProtectedValley() noexcept;
    ~ProtectedValley() noexcept;

protected:
    void ApplyImmediateEffects( const GameModel& model ) override;
};
}
