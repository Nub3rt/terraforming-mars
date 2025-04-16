#include "deimos_down.hpp"

#include "../event_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
DeimosDown::DeimosDown( const GameModel& model ) noexcept :
    EventCard( model, CardID::DEIMOS_DOWN, 31 ) {
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
