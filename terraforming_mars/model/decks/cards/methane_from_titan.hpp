#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class MethaneFromTitan : public AutomatedCard
{
public:
    MethaneFromTitan( const GameModel& model ) noexcept;
    ~MethaneFromTitan() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
