#include "giant_ice_asteroid.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
GiantIceAsteroid::GiantIceAsteroid( const GameModel& model ) noexcept :
    EventCard( model, CardID::GIANT_ICE_ASTEROID, 36 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EVENT );
}

GiantIceAsteroid::~GiantIceAsteroid() noexcept {}

void GiantIceAsteroid::ApplyImmediateEffects() {
    _owner->RaiseTemperature();
    _owner->RaiseTemperature();
    _owner->DestroyResource( Resource::PLANTS, 6 );

    _owner->PlaceOcean();
    _owner->PlaceOcean();
}
}
