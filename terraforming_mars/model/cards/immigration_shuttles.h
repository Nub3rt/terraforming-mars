#pragma once

#include "../automated_card.h"
#include "../game_model.h"

namespace model::decks::cards
{
class ImmigrationShuttles : public AutomatedCard
{
public:
    ImmigrationShuttles( const GameModel& model ) noexcept;
    ~ImmigrationShuttles() noexcept;

protected:
    void ApplyImmediateEffects() override;
    int DoCountVPs() const override;
};
}
