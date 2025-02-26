#include "imported_ghg.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
ImportedGhg::ImportedGhg( const GameModel& model ) noexcept :
    EventCard( model, CardID::IMPORTED_GHG, 7 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EARTH );
    AddTag( Tag::EVENT );
}

ImportedGhg::~ImportedGhg() noexcept {}

void ImportedGhg::ApplyImmediateEffects() {
    _owner->GainResource( Resource::HEAT, 3 );
    _owner->GainResourceProduction( Resource::HEAT, 1 );
}
}
