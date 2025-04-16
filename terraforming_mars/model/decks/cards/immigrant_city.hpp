#pragma once

#include "../active_card_with_effect.hpp"
#include "../../game_model.hpp"

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
