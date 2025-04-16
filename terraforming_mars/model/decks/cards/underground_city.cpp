#include "underground_city.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
