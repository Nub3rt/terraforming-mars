#include "fueled_generators.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
FueledGenerators::FueledGenerators( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::FUELED_GENERATORS, 1 ) {
    AddTag( Tag::BUILDING );
    AddTag( Tag::POWER );
}

FueledGenerators::~FueledGenerators() noexcept {}

bool FueledGenerators::SatisfiesRequirements() const {
    return _owner->GetResourceProduction( Resource::CREDIT ) >= 1 - 5;
}

void FueledGenerators::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::CREDIT, 1 );

    _owner->GainResourceProduction( Resource::ENERGY, 1 );
}
}
