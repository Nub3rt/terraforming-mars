#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class NitrophilicMoss : public AutomatedCard
{
public:
    NitrophilicMoss( const GameModel& model ) noexcept;
    ~NitrophilicMoss() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
