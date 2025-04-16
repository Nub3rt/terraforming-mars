#include "wave_power.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
