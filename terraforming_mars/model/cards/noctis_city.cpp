#include "noctis_city.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
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
    return _owner->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void NoctisCity::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->GainResourceProduction( Resource::CREDIT, 3 );

    _owner->PlaceNoctisCity();
}
}
