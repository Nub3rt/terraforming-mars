#include "eos_chasma_national_park.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
EosChasmaNationalPark::EosChasmaNationalPark( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::EOS_CHASMA_NATIONAL_PARK, 16 ) {
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
