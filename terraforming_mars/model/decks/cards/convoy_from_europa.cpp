#include "convoy_from_europa.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
ConvoyFromEuropa::ConvoyFromEuropa( const GameModel& model ) noexcept :
    EventCard( model, CardID::CONVOY_FROM_EUROPA, 15 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EVENT );
}

ConvoyFromEuropa::~ConvoyFromEuropa() noexcept {}

void ConvoyFromEuropa::ApplyImmediateEffects() {
    _owner->DrawCard();

    _owner->PlaceOcean();
}
}
