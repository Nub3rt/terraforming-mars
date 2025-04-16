#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

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
