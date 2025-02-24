#pragma once

#include "../event_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class ImportedGhg : public EventCard
{
public:
    ImportedGhg( const GameModel& model ) noexcept;
    ~ImportedGhg() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
