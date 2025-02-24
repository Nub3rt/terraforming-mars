#include "giant_space_mirror.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
GiantSpaceMirror::GiantSpaceMirror( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::GIANT_SPACE_MIRROR, 17, false, true ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::POWER );
}

GiantSpaceMirror::~GiantSpaceMirror() noexcept {}

void GiantSpaceMirror::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::ENERGY, 3 );
}
}
