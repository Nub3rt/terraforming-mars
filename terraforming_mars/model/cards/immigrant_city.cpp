#include "immigrant_city.h"

#include "../active_card_with_effect.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
ImmigrantCity::ImmigrantCity( const GameModel& model ) noexcept :
    ActiveCardWithEffect( model, CardID::IMMIGRANT_CITY, 13, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::CITY );
}

ImmigrantCity::~ImmigrantCity() noexcept {}

bool ImmigrantCity::SatisfiesRequirements() const {
    return _owner->GetResourceProduction( Resource::CREDIT ) >= 2 && _owner->GetResourceProduction( Resource::ENERGY ) >= 1 && _model.IsCityPlaceable();
}

void ImmigrantCity::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::CREDIT, 2 );
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->PlaceCity();
}

void ImmigrantCity::DoAfterAnyonePlacesCity() {
    _owner->GainResourceProduction( Resource::CREDIT, 1 );
}
}
