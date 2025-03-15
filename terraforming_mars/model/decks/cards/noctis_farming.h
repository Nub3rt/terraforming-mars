#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class NoctisFarming : public AutomatedCard
{
public:
    NoctisFarming( const GameModel& model ) noexcept;
    ~NoctisFarming() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
