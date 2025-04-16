#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

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
