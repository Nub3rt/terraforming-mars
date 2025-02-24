#include "algae.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
Algae::Algae( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::ALGAE, 10, false, false ) {
    AddTag( Tag::PLANT );
}

Algae::~Algae() noexcept {}

bool Algae::SatisfiesRequirements() const {
    return _model.OceanCount() >= 5;
}

void Algae::ApplyImmediateEffects() {
    _owner->GainResource( Resource::PLANTS, 1 );
    _owner->GainResourceProduction( Resource::PLANTS, 2 );
}
}
