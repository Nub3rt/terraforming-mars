#include "tectonic_stress_power.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
TectonicStressPower::TectonicStressPower( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::TECTONIC_STRESS_POWER, 18 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

TectonicStressPower::~TectonicStressPower() noexcept {}

bool TectonicStressPower::SatisfiesRequirements() const {
    return _holder->GetTagCount( Tag::SCIENCE ) >= 2;
}

void TectonicStressPower::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 3 );
}

int TectonicStressPower::DoCountVPs() const {
    return 1;
}
}
