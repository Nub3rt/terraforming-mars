#include "card.h"

#include <stdexcept>

#include "card_id.h"
#include "../game_model.h"
#include "../player.h"
#include "../resource.h"

namespace model::decks
{
Card::Card( const GameModel& model, CardID card_id, int base_cost ) noexcept :
    _card_id( card_id ), _base_cost( base_cost ), _tags(), _model( model ),
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
    if ( HasTag( Tag::BUILDING ) )
        max_pay_amount = _holder->GetMaxPayAmountForBuilding();
    else if ( HasTag( Tag::SPACE ) )
        max_pay_amount = _holder->GetMaxPayAmountForSpace();
    else
        max_pay_amount = _holder->GetResource( Resource::CREDIT );

    return max_pay_amount >= GetCost() && SatisfiesRequirements();
}

void Card::Play() {
    _owner = _holder;
    ApplyImmediateEffects();
}

int Card::CountVPs() const {
    if ( _owner == nullptr )
        throw std::logic_error( "Card::CountVPs: card has no owner!" );

    return DoCountVPs();
}

void Card::AddTag( Tag tag ) {
    ++_tags[ +tag ];
}

bool Card::SatisfiesRequirements() const { return true; }

void Card::ApplyImmediateEffects() {}

int Card::DoCountVPs() const { return 0; }
}
