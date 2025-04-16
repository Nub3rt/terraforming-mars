#include "lichen.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
Lichen::Lichen( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::LICHEN, 7 ) {
    AddTag( Tag::PLANT );
}

Lichen::~Lichen() noexcept {}

bool Lichen::SatisfiesRequirements() const {
    return _model.Temperature() >= -24;
}

void Lichen::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::PLANTS, 1 );
}
}
