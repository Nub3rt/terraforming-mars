#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class Archaebacteria : public AutomatedCard
{
public:
    Archaebacteria( const GameModel& model ) noexcept;
    ~Archaebacteria() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
};
}
