#include "cloud_seeding.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
CloudSeeding::CloudSeeding( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::CLOUD_SEEDING, 11 ) {}

CloudSeeding::~CloudSeeding() noexcept {}

bool CloudSeeding::SatisfiesRequirements() const {
    return _model.OceanCount() >= 3 && _owner->GetResourceProduction( Resource::CREDIT ) >= 1 - 5;
}

void CloudSeeding::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::CREDIT, 1 );

    _owner->GainResourceProduction( Resource::PLANTS, 2 );
    _owner->DestroyResourceProduction( Resource::HEAT, 1 );
}
}
