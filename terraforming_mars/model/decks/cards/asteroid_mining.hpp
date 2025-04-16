#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class AsteroidMining : public AutomatedCard
{
public:
    AsteroidMining( const GameModel& model ) noexcept;
    ~AsteroidMining() noexcept;

protected:
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
