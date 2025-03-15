#include "insects.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

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
