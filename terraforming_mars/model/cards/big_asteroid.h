#pragma once

#include "../event_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class BigAsteroid : public EventCard
{
public:
    BigAsteroid( const GameModel& model ) noexcept;
    ~BigAsteroid() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
