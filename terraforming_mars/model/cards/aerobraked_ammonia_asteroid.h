#pragma once

#include "../event_card.h"
#include "../game_model.h"

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
