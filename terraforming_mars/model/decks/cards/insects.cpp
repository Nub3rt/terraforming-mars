#include "insects.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
Insects::Insects( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::INSECTS, 9 ) {
    AddTag( Tag::MICROBE );
}

Insects::~Insects() noexcept {}

bool Insects::SatisfiesRequirements() const {
    return _model.Oxygen() >= 6;
}

void Insects::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::PLANTS, _owner->GetTagCount( Tag::PLANT ) );
}
}
