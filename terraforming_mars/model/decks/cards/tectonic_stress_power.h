#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class TectonicStressPower : public AutomatedCard
{
public:
    TectonicStressPower( const GameModel& model ) noexcept;
    ~TectonicStressPower() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
