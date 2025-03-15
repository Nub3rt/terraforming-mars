#include "lake_marineris.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
LakeMarineris::LakeMarineris( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::LAKE_MARINERIS, 18 ) {}

LakeMarineris::~LakeMarineris() noexcept {}

bool LakeMarineris::SatisfiesRequirements() const {
    return _model.Temperature() >= 0;
}

void LakeMarineris::ApplyImmediateEffects() {
    _owner->PlaceOcean();
    _owner->PlaceOcean();
}

int LakeMarineris::DoCountVPs() const {
    return 2;
}
}
