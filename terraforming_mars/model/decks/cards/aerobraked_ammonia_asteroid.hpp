#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class AerobrakedAmmoniaAsteroid : public EventCard
{
public:
    AerobrakedAmmoniaAsteroid( const GameModel& model ) noexcept;
    ~AerobrakedAmmoniaAsteroid() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
