#pragma once

#include "../event_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class ConvoyFromEuropa : public EventCard
{
public:
    ConvoyFromEuropa( const GameModel& model ) noexcept;
    ~ConvoyFromEuropa() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
