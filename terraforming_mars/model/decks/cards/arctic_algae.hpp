#pragma once

#include "../active_card_with_effect.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class ArcticAlgae : public ActiveCardWithEffect
{
public:
    ArcticAlgae( const GameModel& model ) noexcept;
    ~ArcticAlgae() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;

    void DoAfterAnyonePlacesOcean() override;
};
}
