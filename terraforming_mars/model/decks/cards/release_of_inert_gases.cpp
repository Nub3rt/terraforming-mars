#include "release_of_inert_gases.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
