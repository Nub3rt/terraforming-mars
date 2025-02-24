#pragma once

#include "../event_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class ImportOfAdvancedGhg : public EventCard
{
public:
    ImportOfAdvancedGhg( const GameModel& model ) noexcept;
    ~ImportOfAdvancedGhg() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
