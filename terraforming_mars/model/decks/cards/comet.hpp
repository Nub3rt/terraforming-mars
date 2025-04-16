#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class Comet : public EventCard
{
public:
    Comet( const GameModel& model ) noexcept;
    ~Comet() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
