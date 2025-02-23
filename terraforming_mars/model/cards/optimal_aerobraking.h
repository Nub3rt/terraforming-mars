#pragma once

#include "../active_card_with_effect.h"

namespace model::decks::cards
{
class OptimalAerobraking : public ActiveCardWithEffect
{
public:
    OptimalAerobraking() noexcept;
    ~OptimalAerobraking() noexcept;

protected:
    void DoAfterYouPlaySpaceEvent() override;
};
}
