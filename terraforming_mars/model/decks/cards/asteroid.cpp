#include "asteroid.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
Asteroid::Asteroid( const GameModel& model ) noexcept :
    EventCard( model, CardID::ASTEROID, 14 ) {
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
