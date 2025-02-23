#include "water_import_from_europa.h"

#include <functional>

#include "../active_card_with_action.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../tag.h"

namespace model::decks::cards
{
WaterImportFromEuropa::WaterImportFromEuropa() noexcept :
    ActiveCardWithAction( CardID::WATER_IMPORT_FROM_EUROPA, 25, false, true ), _action_credit_cost( 12 ) {
    AddTag( Tag::SPACE );
    AddTag( Tag::JOVIAN );
}

WaterImportFromEuropa::~WaterImportFromEuropa() noexcept {}

bool WaterImportFromEuropa::CanBeUsed() const {
    return _owner->GetMaxPayAmountForSpace() >= _action_credit_cost;
}

void WaterImportFromEuropa::DoUseAction( const GameModel& model ) {
    _owner->ConfirmTitaniumPayment( _action_credit_cost, [ this ]() { _owner->PlaceOcean(); } );
}

int WaterImportFromEuropa::DoCountVPs( const GameModel& model ) const {
    return _owner->GetTagCount( Tag::JOVIAN );
}
}
