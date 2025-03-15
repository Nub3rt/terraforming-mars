#pragma once

#include "../active_card_with_effect.h"
#include "../../game_model.h"

namespace model::decks::cards
{
class ImmigrantCity : public ActiveCardWithEffect
{
public:
    ImmigrantCity( const GameModel& model ) noexcept;
    ~ImmigrantCity() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;

    void DoAfterAnyonePlacesCity() override;
};
}
