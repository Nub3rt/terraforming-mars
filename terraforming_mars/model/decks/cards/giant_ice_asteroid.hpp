#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

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
