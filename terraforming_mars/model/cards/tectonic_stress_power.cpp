#include "tectonic_stress_power.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
TectonicStressPower::TectonicStressPower( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::TECTONIC_STRESS_POWER, 18, true, false ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

TectonicStressPower::~TectonicStressPower() noexcept {}

bool TectonicStressPower::SatisfiesRequirements() const {
    return _owner->GetTagCount( Tag::SCIENCE ) >= 2;
}

void TectonicStressPower::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 3 );
}

int TectonicStressPower::DoCountVPs() const {
    return 1;
}
}
