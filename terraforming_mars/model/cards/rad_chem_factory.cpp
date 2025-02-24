#include "rad_chem_factory.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
RadChemFactory::RadChemFactory( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::RAD_CHEM_FACTORY, 8, true, false ) {
    AddTag( Tag::BUILDING );
}

RadChemFactory::~RadChemFactory() noexcept {}

bool RadChemFactory::SatisfiesRequirements() const {
    return _owner->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void RadChemFactory::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->RaiseTR( 2 );
}
}
