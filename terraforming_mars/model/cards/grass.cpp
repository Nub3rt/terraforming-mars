#include "grass.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
Grass::Grass( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::GRASS, 11, false, false ) {
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
