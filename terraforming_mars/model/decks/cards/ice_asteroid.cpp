#include "ice_asteroid.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
