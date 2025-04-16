#include "adapted_lichen.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
