#include "aquifer_pumping.h"

#include <functional>

#include "../active_card_with_action.h"
#include "../card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"
#include "../tag.h"

namespace model::decks::cards
{
AquiferPumping::AquiferPumping( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::AQUIFER_PUMPING, 18, true, false ), _action_credit_cost( 8 ) {
    AddTag( Tag::BUILDING );
}

AquiferPumping::~AquiferPumping() noexcept {}

bool AquiferPumping::CanBeUsed() const {
    return _owner->GetMaxPayAmountForBuilding() >= _action_credit_cost;
}

void AquiferPumping::DoUseAction() {
    _owner->ConfirmSteelPayment( _action_credit_cost, [ this ]() { _owner->PlaceOcean(); } );
}
}
