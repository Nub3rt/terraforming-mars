#include "active_card_with_action.h"

#include <stdexcept>

#include "active_card.h"
#include "availability.h"
#include "card_id.h"

namespace model::decks
{
ActiveCardWithAction::ActiveCardWithAction( CardID card_id, int base_cost, bool is_building, bool is_space ) noexcept :
    ActiveCard( card_id, base_cost, is_building, is_space ), _used_this_generation( false ) {
}

ActiveCardWithAction::~ActiveCardWithAction() noexcept {}

bool ActiveCardWithAction::IsActiveWithAction() const noexcept { return true; }

Availability ActiveCardWithAction::Availability() const {
    if ( _owner == nullptr )
        throw std::logic_error( "ActiveCardWithAction::Availability: called without having an owner!" );

    if ( _used_this_generation )
        return Availability::USED;

    if ( CanBeUsed() )
        return Availability::CAN_BE_USED;
    else
        return Availability::NOT_USABLE;
}

void ActiveCardWithAction::UseAction( const GameModel& model ) {
    if ( Availability() != Availability::CAN_BE_USED )
        throw std::logic_error( "ActiveCardWithAction::UseAction: card cannot be used at this time!" );

    DoUseAction( model );
    _used_this_generation = true;
}

inline void ActiveCardWithAction::NextGenerationStarted() noexcept { _used_this_generation = false; }
}
