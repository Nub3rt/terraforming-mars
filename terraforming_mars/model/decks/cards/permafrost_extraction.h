#pragma once

#include "../event_card.h"
#include "../../game_model.h"

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
