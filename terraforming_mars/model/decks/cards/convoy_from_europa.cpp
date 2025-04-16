#include "convoy_from_europa.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
