#pragma once

#include "../event_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class ImportedNitrogen : public EventCard
{
public:
    ImportedNitrogen( const GameModel& model ) noexcept;
    ~ImportedNitrogen() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
