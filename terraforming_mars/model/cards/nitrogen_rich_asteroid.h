#pragma once

#include "../event_card.h"

namespace model::decks::cards
{
class NitrogenRichAsteroid : public EventCard
{
public:
    NitrogenRichAsteroid() noexcept;
    ~NitrogenRichAsteroid() noexcept;

protected:
    void ApplyImmediateEffects( const GameModel& _model ) override;
};
}
