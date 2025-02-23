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
    bool SatisfiesRequirements( const GameModel& _model ) const override;
    void ApplyImmediateEffects( const GameModel& _model ) override;

    void DoAfterAnyonePlacesOcean() override;
};
}
