#include "release_of_inert_gases.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
ReleaseOfInertGases::ReleaseOfInertGases( const GameModel& model ) noexcept :
    EventCard( model, CardID::RELEASE_OF_INERT_GASES, 14 ) {
    AddTag( Tag::EVENT );
}

ReleaseOfInertGases::~ReleaseOfInertGases() noexcept {}

void ReleaseOfInertGases::ApplyImmediateEffects() {
    _owner->RaiseTR( 2 );
}
}
