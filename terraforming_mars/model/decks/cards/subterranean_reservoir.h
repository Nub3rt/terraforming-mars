#pragma once

#include "../event_card.h"
#include "../../game_model.h"

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
