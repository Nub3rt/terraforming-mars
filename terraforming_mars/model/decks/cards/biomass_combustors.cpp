#include "biomass_combustors.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
BiomassCombustors::BiomassCombustors( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::BIOMASS_COMBUSTORS, 4 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

BiomassCombustors::~BiomassCombustors() noexcept {}

bool BiomassCombustors::SatisfiesRequirements() const {
    return _model.Oxygen() >= 6;
}

void BiomassCombustors::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 2 );
    _owner->DestroyResourceProduction( Resource::PLANTS, 1 );
}

int BiomassCombustors::DoCountVPs() const {
    return -1;
}
}
