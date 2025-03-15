#pragma once

#include "../event_card.h"
#include "../../game_model.h"

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
