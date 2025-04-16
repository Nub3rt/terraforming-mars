#include "moss.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
Moss::Moss( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::MOSS, 4 ) {
    AddTag( Tag::PLANT );
}

Moss::~Moss() noexcept {}

bool Moss::SatisfiesRequirements() const {
    return _model.OceanCount() >= 3 && _holder->GetResource( Resource::PLANTS ) >= 1;
}

void Moss::ApplyImmediateEffects() {
    _owner->LoseResource( Resource::PLANTS, 1 );

    _owner->GainResourceProduction( Resource::PLANTS, 1 );
}
}
