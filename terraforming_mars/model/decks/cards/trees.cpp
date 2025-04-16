#include "trees.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
