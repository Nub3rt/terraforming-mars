#pragma once

#include "../event_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class DeimosDown : public EventCard
{
public:
    DeimosDown( const GameModel& model ) noexcept;
    ~DeimosDown() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
