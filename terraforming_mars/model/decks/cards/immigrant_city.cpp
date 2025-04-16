#include "immigrant_city.hpp"

#include "../active_card_with_effect.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
ImmigrantCity::ImmigrantCity( const GameModel& model ) noexcept :
    ActiveCardWithEffect( model, CardID::IMMIGRANT_CITY, 13 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::CITY );
}

ImmigrantCity::~ImmigrantCity() noexcept {}

bool ImmigrantCity::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::CREDIT ) >= 2 - 5 && _holder->GetResourceProduction( Resource::ENERGY ) >= 1 && _model.IsCityPlaceable( _holder );
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
