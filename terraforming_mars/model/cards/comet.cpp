#include "comet.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
Comet::Comet( const GameModel& model ) noexcept :
    EventCard( model, CardID::COMET, 21, false, true ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EVENT );
}

Comet::~Comet() noexcept {}

void Comet::ApplyImmediateEffects() {
    _owner->RaiseTemperature();
    _owner->DestroyResource( Resource::PLANTS, 3 );

    _owner->PlaceOcean();
}
}
