#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class AdaptedLichen : public AutomatedCard
{
public:
    AdaptedLichen( const GameModel& model ) noexcept;
    ~AdaptedLichen() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
