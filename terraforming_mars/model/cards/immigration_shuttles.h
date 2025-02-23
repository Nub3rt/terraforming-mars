#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class ImmigrationShuttles : public AutomatedCard
{
public:
    ImmigrationShuttles() noexcept;
    ~ImmigrationShuttles() noexcept;

protected:
    void ApplyImmediateEffects( const GameModel& model ) override;
    int DoCountVPs( const GameModel& model ) const override;
};
}
