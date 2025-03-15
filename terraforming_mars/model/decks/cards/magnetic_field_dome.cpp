#include "magnetic_field_dome.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
MagneticFieldDome::MagneticFieldDome( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::MAGNETIC_FIELD_DOME, 5 ) {
    AddTag( Tag::BUILDING );
}

MagneticFieldDome::~MagneticFieldDome() noexcept {}

bool MagneticFieldDome::SatisfiesRequirements() const {
    return _owner->GetResourceProduction( Resource::ENERGY ) >= 2;
}

void MagneticFieldDome::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 2 );

    _owner->RaiseTR( 1 );
    _owner->GainResourceProduction( Resource::PLANTS, 1 );
}
}
