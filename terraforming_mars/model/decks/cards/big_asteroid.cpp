#include "big_asteroid.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
BigAsteroid::BigAsteroid( const GameModel& model ) noexcept :
    EventCard( model, CardID::BIG_ASTEROID, 27 ) {
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
