#include "lichen.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
Lichen::Lichen( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::LICHEN, 7 ) {
    AddTag( Tag::PLANT );
}

Lichen::~Lichen() noexcept {}

bool Lichen::SatisfiesRequirements() const {
    return _model.Temperature() >= -24;
}

void Lichen::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::PLANTS, 1 );
}
}
