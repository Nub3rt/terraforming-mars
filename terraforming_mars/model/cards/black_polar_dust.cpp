#include "black_polar_dust.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
BlackPolarDust::BlackPolarDust( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::BLACK_POLAR_DUST, 15, false, false ) {}

BlackPolarDust::~BlackPolarDust() noexcept {}

bool BlackPolarDust::SatisfiesRequirements() const {
    return _owner->GetResourceProduction( Resource::CREDIT ) >= 2;
}

void BlackPolarDust::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::CREDIT, 2 );

    _owner->GainResourceProduction( Resource::HEAT, 3 );

    _owner->PlaceOcean();
}
}
