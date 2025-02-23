#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class EnergySaving : public AutomatedCard
{
public:
    EnergySaving() noexcept;
    ~EnergySaving() noexcept;

protected:
    void ApplyImmediateEffects( const GameModel& _model ) override;
};
}
