#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

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
