#include "strip_mine.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
StripMine::StripMine( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::STRIP_MINE, 25 ) {
    AddTag( Tag::BUILDING );
}

StripMine::~StripMine() noexcept {}

bool StripMine::SatisfiesRequirements() const {
    return _owner->GetResourceProduction( Resource::ENERGY ) >= 2;
}

void StripMine::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 2 );

    _owner->RaiseOxygen();
    _owner->RaiseOxygen();
    _owner->GainResourceProduction( Resource::STEEL, 2 );
    _owner->GainResourceProduction( Resource::TITANIUM, 2 );
}
}
