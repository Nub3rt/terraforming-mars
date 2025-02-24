#pragma once

#include "../event_card.h"
#include "../game_model.h"

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
