#include "protected_valley.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

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
