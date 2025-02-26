#include "trees.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
Trees::Trees( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::TREES, 13 ) {
    AddTag( Tag::PLANT );
}

Trees::~Trees() noexcept {}

bool Trees::SatisfiesRequirements() const {
    return _model.Temperature() >= -4;
}

void Trees::ApplyImmediateEffects() {
    _owner->GainResource( Resource::PLANTS, 1 );
    _owner->GainResourceProduction( Resource::PLANTS, 3 );
}

int Trees::DoCountVPs() const {
    return 1;
}
}
