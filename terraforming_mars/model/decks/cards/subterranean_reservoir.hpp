#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class SubterraneanReservoir : public EventCard
{
public:
    SubterraneanReservoir( const GameModel& model ) noexcept;
    ~SubterraneanReservoir() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
