#include "noctis_city.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
NoctisCity::NoctisCity( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::NOCTIS_CITY, 18 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::CITY );
}

NoctisCity::~NoctisCity() noexcept {}

bool NoctisCity::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void NoctisCity::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->GainResourceProduction( Resource::CREDIT, 3 );

    _owner->PlaceNoctisCity();
}
}
