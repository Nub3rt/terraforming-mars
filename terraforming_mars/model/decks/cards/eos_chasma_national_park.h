#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class EosChasmaNationalPark : public AutomatedCard
{
public:
    EosChasmaNationalPark( const GameModel& model ) noexcept;
    ~EosChasmaNationalPark() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
