#include "protected_valley.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
ProtectedValley::ProtectedValley( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::PROTECTED_VALLEY, 23 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::PLANT );
}

ProtectedValley::~ProtectedValley() noexcept {}

void ProtectedValley::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::CREDIT, 2 );

    _owner->PlaceGreeneryOnOcean();
}
}
