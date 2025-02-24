#include "methane_from_titan.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
MethaneFromTitan::MethaneFromTitan( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::METHANE_FROM_TITAN, 28, false, true ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::JOVIAN );
}

MethaneFromTitan::~MethaneFromTitan() noexcept {}

bool MethaneFromTitan::SatisfiesRequirements() const {
    return _model.Oxygen() >= 2;
}

void MethaneFromTitan::ApplyImmediateEffects() {
    _owner->GainResourceProduction( Resource::PLANTS, 2 );
    _owner->GainResourceProduction( Resource::HEAT, 2 );
}

int MethaneFromTitan::DoCountVPs() const {
    return 2;
}
}
