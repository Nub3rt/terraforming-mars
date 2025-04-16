#include "open_city.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
OpenCity::OpenCity( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::OPEN_CITY, 23 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::CITY );
}

OpenCity::~OpenCity() noexcept {}

bool OpenCity::SatisfiesRequirements() const {
    return _model.Oxygen() >= 12 && _model.IsCityPlaceable( _holder ) && _holder->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void OpenCity::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->GainResource( Resource::PLANTS, 2 );
    _owner->GainResourceProduction( Resource::CREDIT, 4 );

    _owner->PlaceCity();
}

int OpenCity::DoCountVPs() const {
    return 1;
}
}
