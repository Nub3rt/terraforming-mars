#include "grass.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
Grass::Grass( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::GRASS, 11 ) {
    AddTag( Tag::PLANT );
}

Grass::~Grass() noexcept {}

bool Grass::SatisfiesRequirements() const {
    return _model.Temperature() >= -16;
}

void Grass::ApplyImmediateEffects() {
    _owner->GainResource( Resource::PLANTS, 3 );
    _owner->GainResourceProduction( Resource::PLANTS, 1 );
}
}
