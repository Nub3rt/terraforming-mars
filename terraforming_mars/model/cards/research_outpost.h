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
    bool SatisfiesRequirements() const override;
    void ApplyImmediateEffects() override;

    int DoModifyCardCost( int cost, const Card* card ) override;
};
}
