#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class PermafrostExtraction : public EventCard
{
public:
    PermafrostExtraction( const GameModel& model ) noexcept;
    ~PermafrostExtraction() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
