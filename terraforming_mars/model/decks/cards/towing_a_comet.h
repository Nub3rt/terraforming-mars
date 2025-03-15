#pragma once

#include "../event_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class TowingAComet : public EventCard
{
public:
    TowingAComet( const GameModel& model ) noexcept;
    ~TowingAComet() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
