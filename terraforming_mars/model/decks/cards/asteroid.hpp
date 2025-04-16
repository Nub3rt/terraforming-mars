#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class Asteroid : public EventCard
{
public:
    Asteroid( const GameModel& model ) noexcept;
    ~Asteroid() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
