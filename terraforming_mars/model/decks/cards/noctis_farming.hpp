#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
