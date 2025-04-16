#include "import_of_advanced_ghg.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
