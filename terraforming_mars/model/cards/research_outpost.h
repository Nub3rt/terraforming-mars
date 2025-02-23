#pragma once

#include "../active_card_with_effect.h"
#include "../game_model.h"


namespace model::decks::cards
{
class ResearchOutpost : public ActiveCardWithEffect
{
public:
    ResearchOutpost() noexcept;
    ~ResearchOutpost() noexcept;

protected:
    bool SatisfiesRequirements( const GameModel& model ) const override;
    void ApplyImmediateEffects( const GameModel& model ) override;

    int DoModifyCardCost( int cost, const Card* card ) override;
};
}
