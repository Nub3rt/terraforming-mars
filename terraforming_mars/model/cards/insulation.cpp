#include "Insulation.h"

#include "../automated_card.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
Insulation::Insulation( const GameModel& model ) noexcept :
    AutomatedCard( model, CardID::INSULATION, 2, false, false ) {}

Insulation::~Insulation() noexcept {}

void Insulation::ApplyImmediateEffects( const GameModel& _model ) {
    int heat_production = _owner->get_heat_production();

    _owner->LoseHeatProduction( heat_production );

    _owner->GainCreditProduction( heat_production );
}
}
