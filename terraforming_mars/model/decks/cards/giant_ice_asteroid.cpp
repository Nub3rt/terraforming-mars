#include "giant_ice_asteroid.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
