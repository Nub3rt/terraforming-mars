#include "card.h"

#include <stdexcept>

#include "card_id.h"
#include "player.h"

namespace model::decks
{
Card::Card( CardID card_id, int base_cost, bool is_building, bool is_space ) noexcept :
    _card_id( card_id ), _base_cost( base_cost ), _is_building( is_building ), _is_space( is_space ),
    _tags(), _tag_count( 0 ),
    _holder( nullptr ), _owner( nullptr ) {
}

Card::~Card() noexcept {}

bool Card::IsEvent() const noexcept { return false; }
bool Card::IsAutomated() const noexcept { return false; }
bool Card::IsActive() const noexcept { return false; }
bool Card::IsActiveWithAction() const noexcept { return false; }
bool Card::IsActiveWithEffect() const noexcept { return false; }

inline CardID Card::get_card_id() const noexcept { return _card_id; }
inline const std::array<Tag, 3>& Card::get_tags() const noexcept { return _tags; }
inline int Card::get_tag_count() const noexcept { return _tag_count; }

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
        max_pay_amount = _holder->get_credit();

    return max_pay_amount >= GetCost() && SatisfiesRequirements( model );
}

void Card::Play( const GameModel& model ) {
    if ( !CanBePlayed( model ) )
        throw std::logic_error( "Card::Play: card cannot be played!" );

    _owner = _holder;
    _owner->PlayCard( this );
    ApplyImmediateEffects( model );
}

int Card::TagsOfType( Tag tag ) const {
    int count = 0;

    for ( int i = 0; i < _tag_count; ++i ) {
        if ( _tags[ i ] == tag )
            ++count;
    }

    return count;
}

int Card::CountVPs( const GameModel& model ) const {
    if ( _owner == nullptr )
        throw std::logic_error( "Card::CountVPs: card has no owner!" );

    return DoCountVPs( model );
}

void Card::AddTag( Tag tag ) {
    _tags[ _tag_count++ ] = tag;
}

int Card::GetCost() const {
    if ( _holder == nullptr )
        throw std::logic_error( "Card::GetCost was called without a holder!" );

    return _holder->CalculateCardCost( _base_cost );
}

bool Card::SatisfiesRequirements( const GameModel& model ) const { return true; }

void Card::ApplyImmediateEffects( const GameModel& model ) {}

int Card::DoCountVPs( const GameModel& model ) const { return 0; }
}
