#include "soletta.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
