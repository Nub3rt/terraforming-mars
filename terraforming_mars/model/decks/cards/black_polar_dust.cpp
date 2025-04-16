#include "black_polar_dust.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
BlackPolarDust::BlackPolarDust( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::BLACK_POLAR_DUST, 15 ) {}

BlackPolarDust::~BlackPolarDust() noexcept {}

bool BlackPolarDust::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::CREDIT ) >= 2 - 5;
}

void BlackPolarDust::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::CREDIT, 2 );

    _owner->GainResourceProduction( Resource::HEAT, 3 );

    _owner->PlaceOcean();
}
}
