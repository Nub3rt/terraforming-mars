#include "Insulation.hpp"

#include "../automated_card.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

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
