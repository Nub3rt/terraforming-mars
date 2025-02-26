#include "nitrogen_rich_asteroid.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
NitrogenRichAsteroid::NitrogenRichAsteroid( const GameModel& model ) noexcept :
    EventCard( model, CardID::NITROGEN_RICH_ASTEROID, 31 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EVENT );
}

NitrogenRichAsteroid::~NitrogenRichAsteroid() noexcept {}

void NitrogenRichAsteroid::ApplyImmediateEffects() {
    _owner->RaiseTR( 2 );
    _owner->RaiseTemperature();
    if ( _owner->GetTagCount( Tag::PLANT ) >= 3 )
        _owner->GainResourceProduction( Resource::PLANTS, 4 );
    else
        _owner->GainResourceProduction( Resource::PLANTS, 1 );
}
}
