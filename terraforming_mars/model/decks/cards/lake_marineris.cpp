#include "lake_marineris.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
