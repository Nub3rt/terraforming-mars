#include "imported_ghg.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
