#include "immigration_shuttles.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
ImmigrationShuttles::ImmigrationShuttles( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::IMMIGRATION_SHUTTLES, 31 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EARTH );
}

ImmigrationShuttles::~ImmigrationShuttles() noexcept {}

void ImmigrationShuttles::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::CREDIT, 5 );
}

int ImmigrationShuttles::DoCountVPs() const {
    return _model.CityCount() / 3;
}
}
