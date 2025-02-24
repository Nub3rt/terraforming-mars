#pragma once

#include "../event_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class GiantIceAsteroid : public EventCard
{
public:
    GiantIceAsteroid( const GameModel& model ) noexcept;
    ~GiantIceAsteroid() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
