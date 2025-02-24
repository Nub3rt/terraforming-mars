#pragma once

#include "../automated_card.h"
#include "../game_model.h"

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
