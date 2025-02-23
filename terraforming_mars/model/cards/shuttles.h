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
    bool SatisfiesRequirements( const GameModel& _model ) const override;
    void ApplyImmediateEffects( const GameModel& _model ) override;
    int DoCountVPs( const GameModel& _model ) const override;

    int DoModifyCardCost( int cost, const Card* card ) override;
};
}
