#include "noctis_city.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
NoctisCity::NoctisCity() noexcept :
    AutomatedCard( CardID::NOCTIS_CITY, 18, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::CITY );
}

NoctisCity::~NoctisCity() noexcept {}

bool NoctisCity::SatisfiesRequirements( const GameModel& model ) const {
    return _owner->get_energy_production() >= 1;
}

void NoctisCity::ApplyImmediateEffects( const GameModel& model ) {
    _owner->LoseEnergyProduction( 1 );

    _owner->GainCreditProduction( 3 );

    _owner->PlaceNoctisCity();
}
}
