#include "magnetic_field_generators.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
