#include "open_city.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
OpenCity::OpenCity( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::OPEN_CITY, 23, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::CITY );
}

OpenCity::~OpenCity() noexcept {}

bool OpenCity::SatisfiesRequirements() const {
    return _model.Oxygen() >= 12 && _model.IsCityPlaceable() && _owner->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void OpenCity::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->GainResource( Resource::PLANTS, 2 );
    _owner->GainResourceProduction( Resource::CREDIT, 4 );

    _owner->PlaceCity();
}

int OpenCity::DoCountVPs() const {
    return 1;
}
}
