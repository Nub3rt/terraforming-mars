#include "rad_chem_factory.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
RadChemFactory::RadChemFactory( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::RAD_CHEM_FACTORY, 8 ) {
    AddTag( Tag::BUILDING );
}

RadChemFactory::~RadChemFactory() noexcept {}

bool RadChemFactory::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void RadChemFactory::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->RaiseTR( 2 );
}
}
