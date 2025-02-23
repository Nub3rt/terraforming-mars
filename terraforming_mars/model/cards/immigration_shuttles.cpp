#include "immigration_shuttles.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
ImmigrationShuttles::ImmigrationShuttles() noexcept :
    AutomatedCard( CardID::IMMIGRATION_SHUTTLES, 31, false, true ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EARTH );
}

ImmigrationShuttles::~ImmigrationShuttles() noexcept {}

void ImmigrationShuttles::ApplyImmediateEffects( const GameModel& model ) {
    _owner->GainCreditProduction( 5 );
}

int ImmigrationShuttles::DoCountVPs( const GameModel& model ) const {
    return model.CityCount() / 3;
}
}
