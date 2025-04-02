#include "cupola_city.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

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
