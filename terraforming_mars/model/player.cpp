#include "player.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <stdexcept>
#include <vector>

#include "decks/card.hpp"
#include "decks/active_card.hpp"
#include "decks/active_card_with_action.hpp"
#include "decks/active_card_with_effect.hpp"
#include "decks/automated_card.hpp"
#include "decks/event_card.hpp"
#include "resource.hpp"
#include "tag.hpp"

namespace model
{
Player::Player( int starting_tr, int starting_resource_production_amount ) : _tr( starting_tr ), _tags(), _resources(), _resource_productions() {
    for ( int& resource_production : _resource_productions )
        resource_production = starting_resource_production_amount;
}

Player::~Player() {
    for ( decks::Card* card : _hand)
        delete card;

    for ( decks::Card* card : _event_cards )
        delete card;

    for ( decks::Card* card : _automated_cards )
        delete card;

    for ( decks::Card* card : _active_cards )
        delete card;
}

void Player::GetTR( int amount ) { _tr += amount; }

int Player::GetResource( Resource resource ) const { return _resources[ +resource ]; }
int Player::GetResourceProduction( Resource resource ) const { return _resource_productions[ +resource ]; }

void Player::GainResource( Resource resource, int amount ) {
    _resources[ +resource ] += amount;
    _on_resource_amount_changed.Invoke( this, resource, amount );
}
void Player::GainResourceProduction( Resource resource, int amount ) {
    _resource_productions[ +resource ] += amount;
    _on_resource_production_amount_changed.Invoke( this, resource, amount );
}
void Player::LoseResource( Resource resource, int amount ) {
    _resources[ +resource ] -= amount;
    _on_resource_amount_changed.Invoke( this, resource, -amount );
}
void Player::LoseResourceProduction( Resource resource, int amount ) {
    _resource_productions[ +resource ] -= amount;
    _on_resource_production_amount_changed.Invoke( this, resource, -amount );
}

void Player::DestroyResource( Resource resource, int amount ) { _on_destroy_resource.Invoke( this, resource, amount ); }
void Player::DestroyResourceProduction( Resource resource, int amount ) { _on_destroy_resource_production.Invoke( this, resource, amount ); }

void Player::PerformProductionPhase() {
    int energy_amount = _resources[ +Resource::ENERGY ];
    if ( energy_amount != 0 ) {
        LoseResource( Resource::ENERGY, energy_amount );
        GainResource( Resource::HEAT, energy_amount );
    }

    GainResource( Resource::CREDIT, _tr + _resource_productions[ +Resource::CREDIT ] );
    for ( int r = +Resource::STEEL; r <= +Resource::MAX; ++r )
        if ( _resource_productions[ r ] > 0 )
            GainResource( static_cast<Resource>( r ), _resource_productions[ r ] );

    for ( decks::ActiveCardWithAction* card : _action_cards )
        card->NextGenerationStarted();
}

int Player::GetTagCount( Tag tag ) {
    return _tags[ +tag ];
}

int Player::GetMaxPayAmountForBuilding() const {
    return _resources[ +Resource::CREDIT ] + _steel_value * _resources[ +Resource::STEEL ];
}

int Player::GetMaxPayAmountForSpace() const {
    return _resources[ +Resource::CREDIT ] + _titanium_value * _resources[ +Resource::TITANIUM ];
}

void Player::ConfirmSteelPayment( int cost, std::function<void()> after_payment ) {
    if ( _resources[ +Resource::STEEL ] == 0 ) {
        LoseResource( Resource::CREDIT, cost );
        after_payment();
        return;
    }

    _on_confirm_steel_payment.Invoke( this, cost, std::move( after_payment ) );
}

void Player::ConfirmTitaniumPayment( int cost, std::function<void()> after_payment ) {
    if ( _resources[ +Resource::TITANIUM ] == 0 ) {
        LoseResource( Resource::CREDIT, cost );
        after_payment();
        return;
    }

    _on_confirm_titanium_payment.Invoke( this, cost, std::move( after_payment ) );
}

void Player::GetCard( decks::Card* card ) {
    card->Buy( this );
    _hand.push_back( card );
}

void Player::SellCard( const decks::Card* card ) {
    auto it_to_card = std::find( _hand.cbegin(), _hand.cend(), card );

    if ( it_to_card == _hand.cend() )
        throw std::logic_error( "Player::SellCard: card was not in hand!" );

    (*it_to_card)->Sell();
    _hand.erase( it_to_card );

    GainResource( Resource::CREDIT, 1 );
}

void Player::PlayCard( const decks::Card* card ) {
    auto it_to_card = std::find( _hand.cbegin(), _hand.cend(), card );

    if ( it_to_card == _hand.cend() )
        throw std::logic_error( "Player::PlayCard: card was not in hand!" );

    if ( !card->CanBePlayed() )
        throw std::logic_error( "Player::PlayCard: card can not be played!" );

    if ( card->HasTag( Tag::BUILDING ) && _resources[ +Resource::STEEL ] > 0 ) {
        ConfirmSteelPayment( card->GetCost(), std::bind( &Player::DoPlayCard, this, *it_to_card ) );
        return;
    }

    if ( card->HasTag( Tag::SPACE ) && _resources[ +Resource::TITANIUM ] > 0 ) {
        ConfirmTitaniumPayment( card->GetCost(), std::bind( &Player::DoPlayCard, this, *it_to_card ) );
        return;
    }

    LoseResource( Resource::CREDIT, card->GetCost() );
    DoPlayCard( *it_to_card );
}

int Player::CalculateCardCost( const decks::Card* card, int base_cost ) const {
    for ( decks::ActiveCardWithEffect* card : _effect_cards )
        base_cost = card->ModifyCardCost( card, base_cost );

    return base_cost;
}

decks::Availability Player::ActionStatus( const decks::Card* card ) const {
    auto it_to_card = std::find( _action_cards.cbegin(), _action_cards.cend(), card );

    if ( it_to_card == _action_cards.cend() )
        throw std::logic_error( "Player::ActionStatus: card was not played by this player!" );

    return (*it_to_card)->Availability();
}

void Player::UseAction( const decks::Card* card ) {
    auto it_to_card = std::find( _action_cards.cbegin(), _action_cards.cend(), card );

    if ( it_to_card == _action_cards.cend() )
        throw std::logic_error( "Player::UseAction: card was not played by this player!" );

    (*it_to_card)->UseAction();
}

void Player::OnEffect( std::function<void( decks::ActiveCardWithEffect* )> effect ) {
    for ( decks::ActiveCardWithEffect* card : _effect_cards )
        effect( card );
}

void Player::DoPlayCard( decks::Card* card ) {
    auto it_to_card = std::find( _hand.cbegin(), _hand.cend(), card );
    if ( it_to_card == _hand.cend() )
        throw std::logic_error( "Player:DoPlayCard: card was not in hand!" );

    _hand.erase( it_to_card );

    if ( card->IsEvent() ) {
        _event_cards.push_back( static_cast<decks::EventCard*>( card ) );
        card->Play();

        if ( card->HasTag( Tag::SPACE ) )
            OnEffect( &decks::ActiveCardWithEffect::AfterYouPlaySpaceEvent );

        return;
    }

    for ( int i = 0; i < +Tag::MAX; ++i )
        _tags[ i ] += card->GetTagCount( static_cast<Tag>( i ) );

    if ( card->IsAutomated() ) {
        _automated_cards.push_back( static_cast<decks::AutomatedCard*>( card ) );
    } else {
        _active_cards.push_back( static_cast<decks::ActiveCard*>( card ) );
        if ( card->IsActiveWithAction() )
            _action_cards.push_back( static_cast<decks::ActiveCardWithAction*>( card ) );
        if ( card->IsActiveWithEffect() )
            _effect_cards.push_back( static_cast<decks::ActiveCardWithEffect*>( card ) );
    }

    card->Play();
}
}
