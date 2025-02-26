#include "Insulation.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
Insulation::Insulation( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::INSULATION, 2 ) {}

Insulation::~Insulation() noexcept {}

void Insulation::ApplyImmediateEffects() {
    int heat_production = _owner->GetResourceProduction( Resource::HEAT );

    _owner->LoseResourceProduction( Resource::HEAT, heat_production );

    _owner->GainResourceProduction( Resource::CREDIT, heat_production );
}
}
