#include "worms.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
Worms::Worms( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::WORMS, 8 ) {
    AddTag( Tag::MICROBE );
}

Worms::~Worms() noexcept {}

bool Worms::SatisfiesRequirements() const {
    return _model.Oxygen() >= 4;
}

void Worms::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::PLANTS, _owner->GetTagCount( Tag::MICROBE ) / 2 );
}
}
