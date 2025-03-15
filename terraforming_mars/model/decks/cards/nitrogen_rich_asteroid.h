#pragma once

#include "../event_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class NitrogenRichAsteroid : public EventCard
{
public:
    NitrogenRichAsteroid( const GameModel& model ) noexcept;
    ~NitrogenRichAsteroid() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
