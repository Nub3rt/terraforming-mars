#include "underground_city.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
UndergroundCity::UndergroundCity( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::UNDERGROUND_CITY, 18 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::CITY );
}

UndergroundCity::~UndergroundCity() noexcept {}

bool UndergroundCity::SatisfiesRequirements() const {
    return _model.IsCityPlaceable( _holder ) && _holder->GetResourceProduction( Resource::ENERGY ) >= 2;
}

void UndergroundCity::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 2 );

    _owner->GainResourceProduction( Resource::STEEL, 2 );

    _owner->PlaceCity();
}
}
