#pragma once

#include "../event_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class MiningExpedition : public EventCard
{
public:
    MiningExpedition( const GameModel& model ) noexcept;
    ~MiningExpedition() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
