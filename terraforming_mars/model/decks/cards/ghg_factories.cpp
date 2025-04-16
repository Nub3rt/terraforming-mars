#include "ghg_factories.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
GhgFactories::GhgFactories( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::GHG_FACTORIES, 11 ) {
    AddTag( Tag::BUILDING );
}

GhgFactories::~GhgFactories() noexcept {}

bool GhgFactories::SatisfiesRequirements() const {
    return _holder->GetResourceProduction( Resource::ENERGY ) >= 1;
}

void GhgFactories::ApplyImmediateEffects() {
    _owner->LoseResourceProduction( Resource::ENERGY, 1 );

    _owner->GainResourceProduction( Resource::HEAT, 4 );
}
}
