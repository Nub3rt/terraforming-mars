#pragma once

#include "../event_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class IceCapMelting : public EventCard
{
public:
    IceCapMelting( const GameModel& model ) noexcept;
    ~IceCapMelting() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
