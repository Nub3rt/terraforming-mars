#include "dust_seals.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
DustSeals::DustSeals( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::DUST_SEALS, 2 ) {}

DustSeals::~DustSeals() noexcept {}

bool DustSeals::SatisfiesRequirements() const {
    return _model.OceanCount() <= 3;
}

void DustSeals::ApplyImmediateEffects() {
}

int DustSeals::DoCountVPs() const {
    return 1;
}
}
