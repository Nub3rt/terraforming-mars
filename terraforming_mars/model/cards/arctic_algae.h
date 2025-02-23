#pragma once

#include "../active_card_with_effect.h"
#include "../game_model.h"

namespace model::decks::cards
{
class ArcticAlgae : public ActiveCardWithEffect
{
public:
    ArcticAlgae() noexcept;
    ~ArcticAlgae() noexcept;

protected:
    bool SatisfiesRequirements( const GameModel& model ) const override;
    void ApplyImmediateEffects( const GameModel& model ) override;

    void DoAfterAnyonePlacesOcean() override;
};
}
