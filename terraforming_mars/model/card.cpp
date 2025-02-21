#include "card.h"

#include <stdexcept>

#include "player.h"

namespace model::decks
{
Card::Card() noexcept : _is_building( false ), _is_space( false ),
    _holder( nullptr ), _owner( nullptr ) {
}

Card::~Card() noexcept {}

bool Card::IsEvent() const noexcept { return false; }
bool Card::IsAutomated() const noexcept { return false; }
bool Card::IsActive() const noexcept { return false; }
bool Card::IsActiveWithAction() const noexcept { return false; }
bool Card::IsActiveWithEffect() const noexcept { return false; }

void Card::Buy( Player* player ) { 
    if ( _holder != nullptr || _owner != nullptr )
        throw std::logic_error( "Card::Buy: card was already bought!" );

    _holder = player;
}

void Card::Sell() {
    if ( _holder == nullptr )
        throw std::logic_error( "Card::Sell: card was not bought!" );

    if ( _owner != nullptr )
        throw std::logic_error( "Card::Sell: card was already played!" );

    _holder = nullptr;
}

bool Card::CanBePlayed( const GameModel& model ) const {
    if ( _holder == nullptr )
        throw std::logic_error( "Card::CanBePlayed: card has no holder!" );

    if ( _owner != nullptr )
        throw std::logic_error( "Card::CanBePlayed: card was already played!" );

    int max_pay_amount;
    if ( _is_building )
        max_pay_amount = _holder->GetMaxPayAmountForBuilding();
    else if ( _is_space )
        max_pay_amount = _holder->GetMaxPayAmountForSpace();
    else
        max_pay_amount = _holder->get_credits();

    return max_pay_amount >= GetCost() && SatisfiesRequirements( model );
}

void Card::Play( const GameModel& model ) {
    if ( !CanBePlayed( model ) )
        throw std::logic_error( "Card::Play: card cannot be played!" );

    _owner = _holder;
    _owner->PlayCard( this );
    ApplyImmediateEffects();
}

int Card::TagsOfType( Tag tag ) const {
    int count = 0;

    for ( int i = 0; i < get_tag_count(); ++i ) {
        if ( get_tags()[ i ] == tag )
            ++count;
    }

    return count;
}

int Card::GetCost() const {
    if ( _holder == nullptr )
        throw std::logic_error( "Card::GetCost was called without a holder" );

    return _holder->CalculateCardCost( get_base_cost() );
}
}
