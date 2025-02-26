#pragma once

#include "../active_card_with_effect.h"
#include "../game_model.h"

namespace model::decks::cards
{
class Shuttles : public ActiveCardWithEffect
{
public:
    Shuttles( const GameModel& model ) noexcept;
    ~Shuttles() noexcept;

protected:
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;

    int DoModifyCardCost( const Card* card, int cost ) override;
};
}
