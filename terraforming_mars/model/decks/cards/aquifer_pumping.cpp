#include "aquifer_pumping.hpp"

#include <functional>

#include "../active_card_with_action.hpp"
#include "../card_id.hpp"
#include "../../game_model.hpp"
#include "../../player.hpp"
#include "../../resource.hpp"
#include "../../tag.hpp"

namespace model::decks::cards
{
AquiferPumping::AquiferPumping( const GameModel& model ) noexcept :
    ActiveCardWithAction( model, CardID::AQUIFER_PUMPING, 18 ), _action_credit_cost( 8 ) {
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
