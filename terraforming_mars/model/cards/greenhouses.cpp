#include "noctis_city.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"
#include "greenhouses.h"

namespace model::decks::cards
{
Greenhouses::Greenhouses( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::GREENHOUSES, 6 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::PLANT );
}

Greenhouses::~Greenhouses() noexcept {}

void Greenhouses::ApplyImmediateEffects() {
    _owner->GainResource( Resource::PLANTS, _model.CityCount() );
}
}
