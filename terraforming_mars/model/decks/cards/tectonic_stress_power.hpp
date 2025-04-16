#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
