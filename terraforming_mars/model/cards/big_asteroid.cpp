#include "big_asteroid.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
BigAsteroid::BigAsteroid( const GameModel& model ) noexcept :
    EventCard( model, CardID::BIG_ASTEROID, 27, false, true ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EVENT );
}

BigAsteroid::~BigAsteroid() noexcept {}

void BigAsteroid::ApplyImmediateEffects() {
    _owner->RaiseTemperature();
    _owner->RaiseTemperature();
    _owner->GainResource( Resource::TITANIUM, 4 );
    _owner->DestroyResource( Resource::PLANTS, 4 );
}
}
