#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

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
