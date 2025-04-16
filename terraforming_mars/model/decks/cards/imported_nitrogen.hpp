#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

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
