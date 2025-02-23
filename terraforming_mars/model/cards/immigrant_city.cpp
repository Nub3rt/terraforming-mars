#include "immigrant_city.h"

#include "../active_card_with_effect.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
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
    return _owner->get_credit_production() >= 2 && _owner->get_energy_production() >= 1 && _model.IsCityPlaceable();
}

void ImmigrantCity::ApplyImmediateEffects() {
    _owner->LoseCreditProduction( 2 );
    _owner->LoseEnergyProduction( 1 );

    _owner->PlaceCity();
}

void ImmigrantCity::DoAfterAnyonePlacesCity() {
    _owner->GainCreditProduction( 1 );
}
}
