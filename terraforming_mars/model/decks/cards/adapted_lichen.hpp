#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
