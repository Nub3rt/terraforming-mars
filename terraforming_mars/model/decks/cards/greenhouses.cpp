#include "noctis_city.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"
#include "greenhouses.hpp"

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
