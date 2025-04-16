#pragma once

#include "../active_card_with_effect.hpp"
#include "../../game_model.hpp"


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

    int DoModifyCardCost( const Card* card, int cost ) override;
};
}
