#include "soil_factory.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
SoilFactory::SoilFactory( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::SOIL_FACTORY, 9, true, false ) {
    AddTag( Tag::BUILDING );
}

SoilFactory::~SoilFactory() noexcept {}

bool SoilFactory::SatisfiesRequirements() const {
    return _owner->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void SoilFactory::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->GainResourceProduction( Resource::PLANTS, 1 );
}

int SoilFactory::DoCountVPs() const {
    return 1;
}
}
