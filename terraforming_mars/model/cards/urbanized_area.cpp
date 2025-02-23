#include "noctis_city.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"
#include "urbanized_area.h"

namespace model::decks::cards
{
UrbanizedArea::UrbanizedArea( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::URBANIZED_AREA, 10, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::CITY );
}

UrbanizedArea::~UrbanizedArea() noexcept {}

bool UrbanizedArea::SatisfiesRequirements() const {
    return _owner->get_energy_production() >= 1 && _model.IsUrbanizedAreaPlaceable();
}

void UrbanizedArea::ApplyImmediateEffects() {
    _owner->LoseEnergyProduction( 1 );

    _owner->GainCreditProduction( 2 );

    _owner->PlaceUrbanizedArea();
}
}
