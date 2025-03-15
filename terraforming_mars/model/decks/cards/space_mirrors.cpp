#include "water_import_from_europa.h"

#include "../active_card_with_action.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"
#include "space_mirrors.h"

namespace model::decks::cards
{
SpaceMirrors::SpaceMirrors( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::SPACE_MIRRORS, 3 ), _action_credit_cost( 7 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::POWER );
}

SpaceMirrors::~SpaceMirrors() noexcept {}

bool SpaceMirrors::CanBeUsed() const {
    return _owner->GetResource( Resource::CREDIT ) >= _action_credit_cost;
}

void SpaceMirrors::DoUseAction() {
    _owner->GainResourceProduction( Resource::ENERGY, 1 );
}
}
