#include "water_splitting_plant.h"

#include "../active_card_with_action.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
WaterSplittingPlant::WaterSplittingPlant( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::WATER_SPLITTING_PLANT, 12, true, false ), _action_energy_cost( 3 ) {
    AddTag( Tag::BUILDING );
}

WaterSplittingPlant::~WaterSplittingPlant() noexcept {}

bool WaterSplittingPlant::SatisfiesRequirements() const {
    return _model.OceanCount() >= 2;
}

bool WaterSplittingPlant::CanBeUsed() const {
    return _owner->get_energy() >= _action_energy_cost;
}

void WaterSplittingPlant::DoUseAction() {
    _owner->LoseEnergy( _action_energy_cost );

    _owner->RaiseOxygen();
}
}
