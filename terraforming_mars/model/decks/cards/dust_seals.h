#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class DustSeals : public AutomatedCard
{
public:
    DustSeals( const GameModel& model ) noexcept;
    ~DustSeals() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
