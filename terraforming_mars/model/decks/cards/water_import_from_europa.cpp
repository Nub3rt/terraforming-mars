#include "water_import_from_europa.hpp"

#include <functional>

#include "../active_card_with_action.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
WaterImportFromEuropa::WaterImportFromEuropa( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::WATER_IMPORT_FROM_EUROPA, 25 ), _action_credit_cost( 12 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::JOVIAN );
}

WaterImportFromEuropa::~WaterImportFromEuropa() noexcept {}

int WaterImportFromEuropa::DoCountVPs() const {
    return _owner->GetTagCount( Tag::JOVIAN );
}

bool WaterImportFromEuropa::CanBeUsed() const {
    return _owner->GetMaxPayAmountForSpace() >= _action_credit_cost;
}

void WaterImportFromEuropa::DoUseAction() {
    _owner->ConfirmTitaniumPayment( _action_credit_cost, [ this ]() { _owner->PlaceOcean(); } );
}
}
