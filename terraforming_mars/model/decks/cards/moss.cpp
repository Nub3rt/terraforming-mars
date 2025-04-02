#include "moss.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../../game_model.h"
#include "../../player.h"
#include "../../resource.h"
#include "../../tag.h"

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
