#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class NitrophilicMoss : public AutomatedCard
{
public:
    NitrophilicMoss( const GameModel& model ) noexcept;
    ~NitrophilicMoss() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
