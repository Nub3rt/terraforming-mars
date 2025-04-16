#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

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
