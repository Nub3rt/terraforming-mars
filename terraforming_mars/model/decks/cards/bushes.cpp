#include "bushes.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
