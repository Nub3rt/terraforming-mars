#pragma once

#include "../event_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class ReleaseOfInertGases : public EventCard
{
public:
    ReleaseOfInertGases( const GameModel& model ) noexcept;
    ~ReleaseOfInertGases() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
