#include "imported_nitrogen.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
