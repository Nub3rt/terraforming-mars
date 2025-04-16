#include "water_splitting_plant.hpp"

#include "../active_card_with_action.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
WaterSplittingPlant::WaterSplittingPlant( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::WATER_SPLITTING_PLANT, 12 ), _action_energy_cost( 3 ) {
    AddTag( Tag::BUILDING );
}

WaterSplittingPlant::~WaterSplittingPlant() noexcept {}

bool WaterSplittingPlant::SatisfiesRequirements() const {
    return _model.OceanCount() >= 2;
}

bool WaterSplittingPlant::CanBeUsed() const {
    return _owner->GetResource( Resource::ENERGY ) >= _action_energy_cost;
}

void WaterSplittingPlant::DoUseAction() {
    _owner->LoseResource( Resource::ENERGY, _action_energy_cost );

    _owner->RaiseOxygen();
}
}
