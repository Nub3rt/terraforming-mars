#include "deimos_down.h"

#include "../event_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
DeimosDown::DeimosDown( const GameModel& model ) noexcept :
    EventCard( model, CardID::DEIMOS_DOWN, 31, false, true ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::EVENT );
}

DeimosDown::~DeimosDown() noexcept {}

void DeimosDown::ApplyImmediateEffects() {
    _owner->RaiseTemperature();
    _owner->RaiseTemperature();
    _owner->RaiseTemperature();
    _owner->GainResource( Resource::STEEL, 4 );
    _owner->DestroyResource( Resource::PLANTS, 8 );
}
}
