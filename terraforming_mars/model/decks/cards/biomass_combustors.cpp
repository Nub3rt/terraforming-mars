#include "biomass_combustors.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
