#include "comet.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
Comet::Comet( const GameModel& model ) noexcept :
    EventCard( model, CardID::COMET, 21 ) {
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
