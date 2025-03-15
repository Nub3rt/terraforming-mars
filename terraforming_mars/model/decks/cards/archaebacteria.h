#pragma once

#include "../automated_card.h"
#include "../../game_model.h"

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
