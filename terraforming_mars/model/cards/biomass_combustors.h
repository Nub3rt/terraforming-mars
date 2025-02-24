#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class BiomassCombustors : public AutomatedCard
{
public:
    BiomassCombustors( const GameModel& model ) noexcept;
    ~BiomassCombustors() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
