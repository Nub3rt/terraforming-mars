#include "noctis_city.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"
#include "urbanized_area.hpp"

namespace model::decks::cards
{
UrbanizedArea::UrbanizedArea( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::URBANIZED_AREA, 10 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::CITY );
}

UrbanizedArea::~UrbanizedArea() noexcept {}

bool UrbanizedArea::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::ENERGY ) >= 1 && _model.IsUrbanizedAreaPlaceable( _holder );
}

void UrbanizedArea::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->GainResourceProduction( Resource::CREDIT, 2 );

    _owner->PlaceUrbanizedArea();
}
}
