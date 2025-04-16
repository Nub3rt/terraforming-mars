#include "towing_a_comet.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
TowingAComet::TowingAComet( const GameModel& model ) noexcept :
    EventCard( model, CardID::TOWING_A_COMET, 23 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EVENT );
}

TowingAComet::~TowingAComet() noexcept {}

void TowingAComet::ApplyImmediateEffects() {
    _owner->RaiseOxygen();
    _owner->GainResource( Resource::PLANTS, 2 );

    _owner->PlaceOcean();
}
}
