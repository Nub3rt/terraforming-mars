#include "kelp_farming.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
KelpFarming::KelpFarming( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::KELP_FARMING, 17 ) {
    AddTag( Tag::PLANT );
}

KelpFarming::~KelpFarming() noexcept {}

bool KelpFarming::SatisfiesRequirements() const {
    return _model.OceanCount() >= 6;
}

void KelpFarming::ApplyImmediateEffects() {
    _owner->GainResource( Resource::PLANTS, 2 );
    _owner->GainResourceProduction( Resource::CREDIT, 2 );
    _owner->GainResourceProduction( Resource::PLANTS, 3 );
}

int KelpFarming::DoCountVPs() const {
    return 1;
}
}
