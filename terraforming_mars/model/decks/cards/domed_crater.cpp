#include "domed_crater.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
DomedCrater::DomedCrater( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::DOMED_CRATER, 24 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::CITY );
}

DomedCrater::~DomedCrater() noexcept {}

bool DomedCrater::SatisfiesRequirements() const {
    return _model.Oxygen() <= 7 && _model.IsCityPlaceable( _holder ) && _holder->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void DomedCrater::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->GainResource( Resource::PLANTS, 3 );
    _owner->GainResourceProduction( Resource::CREDIT, 3 );

    _owner->PlaceCity();
}

int DomedCrater::DoCountVPs() const {
    return 1;
}
}
