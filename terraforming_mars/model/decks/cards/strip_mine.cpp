#include "strip_mine.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
StripMine::StripMine( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::STRIP_MINE, 25 ) {
    AddTag( Tag::BUILDING );
}

StripMine::~StripMine() noexcept {}

bool StripMine::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::ENERGY ) >= 2;
}

void StripMine::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 2 );

    _owner->RaiseOxygen();
    _owner->RaiseOxygen();
    _owner->GainResourceProduction( Resource::STEEL, 2 );
    _owner->GainResourceProduction( Resource::TITANIUM, 2 );
}
}
