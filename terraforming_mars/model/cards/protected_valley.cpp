#include "protected_valley.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
ProtectedValley::ProtectedValley( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::PROTECTED_VALLEY, 23, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::PLANT );
}

ProtectedValley::~ProtectedValley() noexcept {}

void ProtectedValley::ApplyImmediateEffects( const GameModel& _model ) {
    _owner->GainCreditProduction( 2 );

    _owner->PlaceGreeneryOnOcean();
}
}
