#include "cupola_city.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
CupolaCity::CupolaCity( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::CUPOLA_CITY, 16 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::CITY );
}

CupolaCity::~CupolaCity() noexcept {}

bool CupolaCity::SatisfiesRequirements() const {
    return _model.Oxygen() <= 9 && _model.IsCityPlaceable( _holder ) && _holder->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void CupolaCity::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->GainResourceProduction( Resource::CREDIT, 3 );

    _owner->PlaceCity();
}
}
