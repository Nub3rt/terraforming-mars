#include "imported_nitrogen.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
ImportedNitrogen::ImportedNitrogen( const GameModel& model ) noexcept :
    EventCard( model, CardID::IMPORTED_NITROGEN, 23 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EARTH );
    AddTag( Tag::EVENT );
}

ImportedNitrogen::~ImportedNitrogen() noexcept {}

void ImportedNitrogen::ApplyImmediateEffects() {
    _owner->RaiseTR( 1 );
    _owner->GainResource( Resource::PLANTS, 4 );
    // _owner->AddResource( Resource::MICROBE, 3 );
    // _owner->AddResource( Resource::ANIMAL, 2 );
}
}
