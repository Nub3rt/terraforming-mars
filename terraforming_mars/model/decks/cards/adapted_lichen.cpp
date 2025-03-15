#include "adapted_lichen.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
AdaptedLichen::AdaptedLichen( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::ADAPTED_LICHEN, 9 ) {
    AddTag( Tag::PLANT );
}

AdaptedLichen::~AdaptedLichen() noexcept {}

void AdaptedLichen::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::PLANTS, 1 );
}
}
