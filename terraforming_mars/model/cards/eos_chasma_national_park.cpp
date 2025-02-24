#include "eos_chasma_national_park.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
EosChasmaNationalPark::EosChasmaNationalPark( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::EOS_CHASMA_NATIONAL_PARK, 16, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::PLANT );
}

EosChasmaNationalPark::~EosChasmaNationalPark() noexcept {}

bool EosChasmaNationalPark::SatisfiesRequirements() const {
    return _model.Temperature() >= -12;
}

void EosChasmaNationalPark::ApplyImmediateEffects() {
    _owner->GainResource( Resource::PLANTS, 3 );
    _owner->GainResourceProduction( Resource::CREDIT, 2 );
    // _owner->AddResource( Resource::ANIMAL, 1 );
}

int EosChasmaNationalPark::DoCountVPs() const {
    return 1;
}
}
