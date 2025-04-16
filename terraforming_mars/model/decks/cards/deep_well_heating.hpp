#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
