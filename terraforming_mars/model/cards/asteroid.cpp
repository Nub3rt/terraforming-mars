#include "asteroid.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
Asteroid::Asteroid( const GameModel& model ) noexcept :
    EventCard( model, CardID::ASTEROID, 14, false, true ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EVENT );
}

Asteroid::~Asteroid() noexcept {}

void Asteroid::ApplyImmediateEffects() {
    _owner->RaiseTemperature();
    _owner->GainResource( Resource::TITANIUM, 2 );
    _owner->DestroyResource( Resource::PLANTS, 3 );
}
}
