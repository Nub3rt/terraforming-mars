#include "heather.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
