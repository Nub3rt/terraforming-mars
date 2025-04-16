#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

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
