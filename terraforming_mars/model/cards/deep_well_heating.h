#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class DeepWellHeating : public AutomatedCard
{
public:
    DeepWellHeating( const GameModel& model ) noexcept;
    ~DeepWellHeating() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
