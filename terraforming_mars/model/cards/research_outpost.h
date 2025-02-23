#pragma once

#include "../active_card_with_effect.h"
#include "../game_model.h"


namespace model::decks::cards
{
class ResearchOutpost : public ActiveCardWithEffect
{
public:
    ResearchOutpost( const GameModel& model ) noexcept;
    ~ResearchOutpost() noexcept;

protected:
    bool SatisfiesRequirements( const GameModel& _model ) const override;
    void ApplyImmediateEffects( const GameModel& _model ) override;

    int DoModifyCardCost( int cost, const Card* card ) override;
};
}
