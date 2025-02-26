#include "wave_power.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
WavePower::WavePower( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::WAVE_POWER, 8 ) {
    AddTag( Tag::POWER );
}

WavePower::~WavePower() noexcept {}

bool WavePower::SatisfiesRequirements() const {
    return _model.OceanCount() >= 3;
}

void WavePower::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 1 );
}

int WavePower::DoCountVPs() const {
    return 1;
}
}
