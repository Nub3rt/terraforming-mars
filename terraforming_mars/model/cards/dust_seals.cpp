#include "dust_seals.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
DustSeals::DustSeals( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::DUST_SEALS, 2, false, false ) {}

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
