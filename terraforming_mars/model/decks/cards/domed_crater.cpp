#include "domed_crater.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

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
