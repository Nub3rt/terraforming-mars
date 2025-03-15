#include "import_of_advanced_ghg.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
ImportOfAdvancedGhg::ImportOfAdvancedGhg( const GameModel& model ) noexcept :
    EventCard( model, CardID::IMPORT_OF_ADVANCED_GHG, 9 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EARTH );
    AddTag( Tag::EVENT );
}

ImportOfAdvancedGhg::~ImportOfAdvancedGhg() noexcept {}

void ImportOfAdvancedGhg::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::HEAT, 2 );
}
}
