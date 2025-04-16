#include "space_mirrors.hpp"

#include "../active_card_with_action.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
