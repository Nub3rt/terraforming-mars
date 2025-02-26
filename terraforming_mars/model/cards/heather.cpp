#include "heather.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
Heather::Heather( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::HEATHER, 6 ) {
    AddTag( Tag::PLANT );
}

Heather::~Heather() noexcept {}

bool Heather::SatisfiesRequirements() const {
    return _model.Temperature() >= -14;
}

void Heather::ApplyImmediateEffects() {
    _owner->GainResource( Resource::PLANTS, 1 );
    _owner->GainResourceProduction( Resource::PLANTS, 1 );
}
}
