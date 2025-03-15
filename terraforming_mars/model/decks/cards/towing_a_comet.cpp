#include "towing_a_comet.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

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
