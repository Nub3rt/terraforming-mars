#include "bushes.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

namespace model::decks::cards
{
Bushes::Bushes( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::BUSHES, 10 ) {
    AddTag( Tag::PLANT );
}

Bushes::~Bushes() noexcept {}

bool Bushes::SatisfiesRequirements() const {
    return _model.Temperature() >= -10;
}

void Bushes::ApplyImmediateEffects() {
    _owner->GainResource( Resource::PLANTS, 2 );
    _owner->GainResourceProduction( Resource::PLANTS, 2 );
}
}
