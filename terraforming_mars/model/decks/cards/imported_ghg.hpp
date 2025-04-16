#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

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
