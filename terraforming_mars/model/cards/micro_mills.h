#pragma once

#include "../automated_card.h"
#include "../game_model.h"

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
