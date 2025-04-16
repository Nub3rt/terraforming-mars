#include "nitrogen_rich_asteroid.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
