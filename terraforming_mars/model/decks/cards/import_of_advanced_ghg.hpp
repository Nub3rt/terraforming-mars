#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

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
