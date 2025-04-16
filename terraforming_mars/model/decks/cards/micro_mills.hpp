#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class MicroMills : public AutomatedCard
{
public:
    MicroMills( const GameModel& model ) noexcept;
    ~MicroMills() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
