#include "algae.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
Algae::Algae( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::ALGAE, 10 ) {
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
