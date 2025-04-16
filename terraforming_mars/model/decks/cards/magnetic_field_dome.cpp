#include "magnetic_field_dome.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
MagneticFieldDome::MagneticFieldDome( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::MAGNETIC_FIELD_DOME, 5 ) {
    AddTag( Tag::BUILDING );
}

MagneticFieldDome::~MagneticFieldDome() noexcept {}

bool MagneticFieldDome::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::ENERGY ) >= 2;
}

void MagneticFieldDome::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 2 );

    _owner->RaiseTR( 1 );
    _owner->GainResourceProduction( Resource::PLANTS, 1 );
}
}
