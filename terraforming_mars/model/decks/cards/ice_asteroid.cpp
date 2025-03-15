#include "ice_asteroid.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
IceAsteroid::IceAsteroid( const GameModel& model ) noexcept :
    EventCard( model, CardID::ICE_ASTEROID, 23 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EVENT );
}

IceAsteroid::~IceAsteroid() noexcept {}

void IceAsteroid::ApplyImmediateEffects() {
    _owner->PlaceOcean();
    _owner->PlaceOcean();
}
}
