#include "soletta.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
Soletta::Soletta( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::SOLETTA, 35 ) {
    AddTag( Tag::SPACE );
}

Soletta::~Soletta() noexcept {}

void Soletta::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::HEAT, 7 );
}
}
