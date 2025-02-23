#include "water_import_from_europa.h"

#include "../active_card_with_action.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"
#include "space_mirrors.h"

namespace model::decks::cards
{
SpaceMirrors::SpaceMirrors() noexcept :
    ActiveCardWithAction( CardID::SPACE_MIRRORS, 3, false, true ), _action_credit_cost( 7 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::POWER );
}

SpaceMirrors::~SpaceMirrors() noexcept {}

bool SpaceMirrors::CanBeUsed() const {
    return _owner->get_credit() >= _action_credit_cost;
}

void SpaceMirrors::DoUseAction( const GameModel& _model ) {
    _owner->GainEnergyProduction( 1 );
}
}
