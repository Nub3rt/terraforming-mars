#include "active_card_with_effect.h"

#include <stdexcept>

#include "active_card.h"
#include "card_id.h"

namespace model::decks
{
ActiveCardWithEffect::ActiveCardWithEffect( const GameModel& model, CardID card_id, int base_cost, bool is_building, bool is_space ) noexcept :
    ActiveCard( model, card_id, base_cost, is_building, is_space ) {
}

ActiveCardWithEffect::~ActiveCardWithEffect() noexcept {}

bool ActiveCardWithEffect::IsActiveWithEffect() const noexcept { return true; }

void ActiveCardWithEffect::AfterAnyonePlacesCity() {
    if ( _owner == nullptr )
        throw std::logic_error( "ActiveCardWithEffect::AfterAnyonePlacesCity: card has no owner!" );

    DoAfterAnyonePlacesCity();
}

void ActiveCardWithEffect::AfterAnyonePlacesOcean() {
    if ( _owner == nullptr )
        throw std::logic_error( "ActiveCardWithEffect::AfterAnyonePlacesOcean: card has no owner!" );

    DoAfterAnyonePlacesOcean();
}

void ActiveCardWithEffect::AfterYouPlaySpaceEvent() {
    if ( _owner == nullptr )
        throw std::logic_error( "ActiveCardWithEffect::AfterYouPlaySpaceEvent: card has no owner!" );

    DoAfterYouPlaySpaceEvent();
}

int ActiveCardWithEffect::ModifyCardCost( int cost, const Card* card ) {
    if ( _owner == nullptr )
        throw std::logic_error( "ActiveCardWithEffect::ModifyCardCost: card has no owner!" );

    return DoModifyCardCost( cost, card );
}

void ActiveCardWithEffect::DoAfterAnyonePlacesCity() {}
void ActiveCardWithEffect::DoAfterAnyonePlacesOcean() {}
void ActiveCardWithEffect::DoAfterYouPlaySpaceEvent() {}
int ActiveCardWithEffect::DoModifyCardCost( int cost, const Card* card ) { return cost; }
}
