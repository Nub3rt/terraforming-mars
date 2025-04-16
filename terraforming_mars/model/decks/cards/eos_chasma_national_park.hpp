#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
