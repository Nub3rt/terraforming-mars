#include "magnetic_field_generators.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
MagneticFieldGenerators::MagneticFieldGenerators( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::MAGNETIC_FIELD_GENERATORS, 20 ) {
    AddTag( Tag::BUILDING );
}

MagneticFieldGenerators::~MagneticFieldGenerators() noexcept {}

bool MagneticFieldGenerators::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::ENERGY ) >= 4;
}

void MagneticFieldGenerators::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 4 );

    _owner->RaiseTR( 3 );
    _owner->GainResourceProduction( Resource::PLANTS, 2 );
}
}
