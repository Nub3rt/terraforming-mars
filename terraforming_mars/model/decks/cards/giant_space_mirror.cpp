#include "giant_space_mirror.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
GiantSpaceMirror::GiantSpaceMirror( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::GIANT_SPACE_MIRROR, 17 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::POWER );
}

GiantSpaceMirror::~GiantSpaceMirror() noexcept {}

void GiantSpaceMirror::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 3 );
}
}
