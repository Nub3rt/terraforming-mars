#include "noctis_city.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
NoctisCity::NoctisCity( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::NOCTIS_CITY, 18, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::CITY );
}

NoctisCity::~NoctisCity() noexcept {}

bool NoctisCity::SatisfiesRequirements() const {
    return _owner->get_energy_production() >= 1;
}

void NoctisCity::ApplyImmediateEffects() {
    _owner->LoseEnergyProduction( 1 );

    _owner->GainCreditProduction( 3 );

    _owner->PlaceNoctisCity();
}
}
