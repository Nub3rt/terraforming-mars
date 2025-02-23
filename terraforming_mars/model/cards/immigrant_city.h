#pragma once

#include "../active_card_with_effect.h"
#include "../game_model.h"

namespace model::decks::cards
{
class ImmigrantCity : public ActiveCardWithEffect
{
public:
    ImmigrantCity() noexcept;
    ~ImmigrantCity() noexcept;

protected:
    bool SatisfiesRequirements( const GameModel& model ) const override;
    void ApplyImmediateEffects( const GameModel& model ) override;

    void DoAfterAnyonePlacesCity() override;
};
}
