#include "ghg_factories.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
GhgFactories::GhgFactories( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::GHG_FACTORIES, 11, true, false ) {
    AddTag( Tag::BUILDING );
}

GhgFactories::~GhgFactories() noexcept {}

bool GhgFactories::SatisfiesRequirements() const {
    return _owner->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void GhgFactories::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->GainResourceProduction( Resource::HEAT, 4 );
}
}
