#include "noctis_city.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"
#include "urbanized_area.h"

namespace model::decks::cards
{
UrbanizedArea::UrbanizedArea() noexcept :
    AutomatedCard( CardID::URBANIZED_AREA, 10, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::CITY );
}

UrbanizedArea::~UrbanizedArea() noexcept {}

bool UrbanizedArea::SatisfiesRequirements( const GameModel& model ) const {
    return _owner->get_energy_production() >= 1 && model.IsUrbanizedAreaPlaceable();
}

void UrbanizedArea::ApplyImmediateEffects( const GameModel& model ) {
    _owner->LoseEnergyProduction( 1 );

    _owner->GainCreditProduction( 2 );

    _owner->PlaceUrbanizedArea();
}
}
