#pragma once

#include "../event_card.hpp"
#include "../../game_model.hpp"

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
