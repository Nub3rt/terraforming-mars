#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class EnergySaving : public AutomatedCard
{
public:
    EnergySaving( const GameModel& model ) noexcept;
    ~EnergySaving() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
