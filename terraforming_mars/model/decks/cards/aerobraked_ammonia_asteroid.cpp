#include "aerobraked_ammonia_asteroid.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
AerobrakedAmmoniaAsteroid::AerobrakedAmmoniaAsteroid( const GameModel& model ) noexcept :
    EventCard( model, CardID::AEROBRAKED_AMMONIA_ASTEROID, 26 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EVENT );
}

AerobrakedAmmoniaAsteroid::~AerobrakedAmmoniaAsteroid() noexcept {}

void AerobrakedAmmoniaAsteroid::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::PLANTS, 1 );
    _owner->GainResourceProduction( Resource::HEAT, 3 );
    // _owner->AddResource( Resource::MICROBE, 2 );
}
}
