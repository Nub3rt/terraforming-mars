#include "nitrogen_rich_asteroid.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
NitrogenRichAsteroid::NitrogenRichAsteroid() noexcept :
    EventCard( CardID::NITROGEN_RICH_ASTEROID, 31, false, true ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EVENT );
}

NitrogenRichAsteroid::~NitrogenRichAsteroid() noexcept {}

void NitrogenRichAsteroid::ApplyImmediateEffects( const GameModel& model ) {
    _owner->RaiseTR( 2 );
    _owner->RaiseTemperature();
    if ( _owner->GetTagCount( Tag::PLANT ) >= 3 )
        _owner->GainPlantsProduction( 4 );
    else
        _owner->GainPlantsProduction( 1 );
}
}
