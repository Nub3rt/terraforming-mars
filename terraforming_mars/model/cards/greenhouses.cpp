#include "noctis_city.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"
#include "greenhouses.h"

namespace model::decks::cards
{
Greenhouses::Greenhouses( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::GREENHOUSES, 6, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::PLANT );
}

Greenhouses::~Greenhouses() noexcept {}

void Greenhouses::ApplyImmediateEffects( const GameModel& _model ) {
    _owner->GainPlants( _model.CityCount() );
}
}
