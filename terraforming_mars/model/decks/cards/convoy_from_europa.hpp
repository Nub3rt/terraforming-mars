#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

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
