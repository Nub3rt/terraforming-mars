#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
