#include "card.h"

#include <stdexcept>

#include "card_id.h"
#include "player.h"

namespace model::decks
{
Card::Card( const GameModel& model, CardID card_id, int base_cost, bool is_building, bool is_space ) noexcept :
    _card_id( card_id ), _base_cost( base_cost ), _is_building( is_building ), _is_space( is_space ),
    _tags(), _tag_count( 0 ), _model( model ),
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
inline bool Card::get_is_building() const noexcept { return _is_building; }
inline bool Card::get_is_space() const noexcept { return _is_space; }

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

int Card::GetCost() const {
    if ( _holder == nullptr )
        throw std::logic_error( "Card::GetCost was called without a holder!" );

    return _holder->CalculateCardCost( this, _base_cost );
}

bool Card::CanBePlayed() const {
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
        max_pay_amount = _holder->GetResource( Resource::CREDIT );

    return max_pay_amount >= GetCost() && SatisfiesRequirements();
}

void Card::Play() {
    if ( !CanBePlayed() )
        throw std::logic_error( "Card::Play: card cannot be played!" );

    _owner = _holder;
    ApplyImmediateEffects();
}

int Card::TagsOfType( Tag tag ) const {
    int count = 0;

    for ( int i = 0; i < _tag_count; ++i ) {
        if ( _tags[ i ] == tag )
            ++count;
    }

    return count;
}

int Card::CountVPs() const {
    if ( _owner == nullptr )
        throw std::logic_error( "Card::CountVPs: card has no owner!" );

    return DoCountVPs();
}

void Card::AddTag( Tag tag ) {
    _tags[ _tag_count++ ] = tag;
}

bool Card::SatisfiesRequirements() const { return true; }

void Card::ApplyImmediateEffects() {}

int Card::DoCountVPs() const { return 0; }
}
