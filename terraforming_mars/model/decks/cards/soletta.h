#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

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
