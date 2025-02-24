#pragma once

#include "../event_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class IceAsteroid : public EventCard
{
public:
    IceAsteroid( const GameModel& model ) noexcept;
    ~IceAsteroid() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
