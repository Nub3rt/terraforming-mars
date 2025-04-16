#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

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
