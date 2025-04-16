#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class Greenhouses : public AutomatedCard
{
public:
    Greenhouses( const GameModel& model ) noexcept;
    ~Greenhouses() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
