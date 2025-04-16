#pragma once

#include "../automated_card.hpp"
#include "../../game_model.hpp"

namespace model::decks::cards
{
class GiantSpaceMirror : public AutomatedCard
{
public:
    GiantSpaceMirror( const GameModel& model ) noexcept;
    ~GiantSpaceMirror() noexcept;

protected:
    void ApplyImmediateEffects() override;
};
}
