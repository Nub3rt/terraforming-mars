#include "cloud_seeding.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
CloudSeeding::CloudSeeding( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::CLOUD_SEEDING, 11 ) {}

CloudSeeding::~CloudSeeding() noexcept {}

bool CloudSeeding::SatisfiesRequirements() const {
    return _model.OceanCount() >= 3 && _holder->GetResourceProduction( Resource::CREDIT ) >= 1 - 5;
}

void CloudSeeding::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::CREDIT, 1 );

    _owner->GainResourceProduction( Resource::PLANTS, 2 );
    _owner->DestroyResourceProduction( Resource::HEAT, 1 );
}
}
