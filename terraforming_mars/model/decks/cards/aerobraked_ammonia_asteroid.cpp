#include "aerobraked_ammonia_asteroid.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

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
