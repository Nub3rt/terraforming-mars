#include "player.h"

#include <algorithm>
#include <array>
#include <functional>
#include <stdexcept>
#include <vector>

#include "decks/card.h"
#include "decks/active_card.h"
#include "decks/active_card_with_action.h"
#include "decks/active_card_with_effect.h"
#include "decks/automated_card.h"
#include "decks/event_card.h"
#include "resource.h"
#include "tag.h"

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

inline const int Player::get_tr() const noexcept { return _tr; }
inline const std::array<int, +Resource::MAX + 1>& Player::get_resources() const noexcept { return _resources; }
inline const std::array<int, +Resource::MAX + 1>& Player::get_resource_productions() const noexcept { return _resource_productions; }
inline int Player::get_steel_value() const noexcept { return _steel_value; }
inline int Player::get_titanium_value() const noexcept { return _titanium_value; }
inline int Player::get_greenery_cost() const noexcept { return _greenery_cost; }
inline int Player::get_temperature_cost() const noexcept { return _temperature_cost; }
inline const std::vector<decks::Card*>& Player::get_hand() const noexcept { return _hand; }
inline const std::vector<decks::EventCard*>& Player::get_event_cards() const noexcept { return _event_cards; }
inline const std::vector<decks::AutomatedCard*>& Player::get_automated_cards() const noexcept { return _automated_cards; }
inline const std::vector<decks::ActiveCardWithAction*>& Player::get_action_cards() const noexcept { return _action_cards; }
inline const std::vector<decks::ActiveCardWithEffect*>& Player::get_effect_cards() const noexcept { return _effect_cards; }

inline void Player::DrawCard() { _on_draw_card.Invoke( this ); }
inline void Player::RaiseTR( int amount ) { _on_raise_tr.Invoke( this, amount ); }
inline void Player::RaiseTemperature() { _on_raise_temperature.Invoke( this ); }
inline void Player::PlaceOcean() { _on_place_ocean.Invoke( this ); }
inline void Player::PlaceOceanOnNonOcean() { _on_place_ocean_on_non_ocean.Invoke( this ); }
inline void Player::RaiseOxygen() { _on_raise_oxygen.Invoke( this ); }
inline void Player::PlaceGreenery() { _on_place_greenery.Invoke( this ); }
inline void Player::PlaceGreeneryOnOcean() { _on_place_greenery_on_ocean.Invoke( this ); }
inline void Player::PlaceCity() { _on_place_city.Invoke( this ); }
inline void Player::PlaceNoctisCity() { _on_place_noctis_city.Invoke( this ); }
inline void Player::PlaceLonelyCity() { _on_place_lonely_city.Invoke( this ); }
inline void Player::PlaceUrbanizedArea() { _on_place_urbanized_area.Invoke( this ); }

inline void Player::GetTR( int amount ) { _tr += amount; }

inline int Player::GetResource( Resource resource ) const { return _resources[ +resource ]; }
inline int Player::GetResourceProduction( Resource resource ) const { return _resource_productions[ +resource ]; }

inline void Player::GainResource( Resource resource, int amount ) {
    _resources[ +resource ] += amount;
    _on_resource_amount_changed.Invoke( this, resource, amount );
}
inline void Player::GainResourceProduction( Resource resource, int amount ) {
    _resource_productions[ +resource ] += amount;
    _on_resource_production_amount_changed.Invoke( this, resource, amount );
}
inline void Player::LoseResource( Resource resource, int amount ) {
    _resources[ +resource ] -= amount;
    _on_resource_amount_changed.Invoke( this, resource, -amount );
}
inline void Player::LoseResourceProduction( Resource resource, int amount ) {
    _resource_productions[ +resource ] -= amount;
    _on_resource_production_amount_changed.Invoke( this, resource, -amount );
}

inline void Player::DestroyResource( Resource resource, int amount ) { _on_destroy_resource.Invoke( this, resource, amount ); }
inline void Player::DestroyResourceProduction( Resource resource, int amount ) { _on_destroy_resource_production.Invoke( this, resource, amount ); }

void Player::PerformProductionPhase() {
    int energy_amount = _resources[ +Resource::ENERGY ];
    LoseResource( Resource::ENERGY, energy_amount );
    GainResource( Resource::HEAT, energy_amount );

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
        LoseResource( Resource::STEEL, cost );
        after_payment();
        return;
    }

    _on_confirm_steel_payment.Invoke( this, cost, std::move( after_payment ) );
}

void Player::ConfirmTitaniumPayment( int cost, std::function<void()> after_payment ) {
    if ( _resources[ +Resource::TITANIUM ] == 0 ) {
        LoseResource( Resource::TITANIUM, cost );
        after_payment();
        return;
    }

    _on_confirm_titanium_payment.Invoke( this, cost, std::move( after_payment ) );
}

void Player::GetCard( decks::Card* card ) {
    _hand.push_back( card );
}

void Player::SellCard( decks::Card* card ) {
    auto it_to_card = std::find( _hand.cbegin(), _hand.cend(), card );

    if ( it_to_card == _hand.cend() )
        throw std::logic_error( "Player::SellCard: card was not in hand!" );

    _hand.erase( it_to_card );

    GainResource( Resource::CREDIT, 1 );
}

void Player::PlayCard( decks::Card* card ) {
    if ( std::find( _hand.cbegin(), _hand.cend(), card ) == _hand.cend() )
        throw std::logic_error( "Player::PlayCard: card was not in hand!" );

    if ( !card->CanBePlayed() )
        throw std::logic_error( "Player::PlayCard: card can not be played!" );

    if ( card->HasTag( Tag::BUILDING ) && _resources[ +Resource::STEEL ] > 0 ) {
        ConfirmSteelPayment( card->GetCost(), std::bind( &Player::DoPlayCard, this, card ) );
        return;
    }

    if ( card->HasTag( Tag::SPACE ) && _resources[ +Resource::TITANIUM ] > 0 ) {
        ConfirmTitaniumPayment( card->GetCost(), std::bind( &Player::DoPlayCard, this, card ) );
        return;
    }

    DoPlayCard( card );
}

int Player::CalculateCardCost( const decks::Card* card, int base_cost ) const {
    for ( decks::ActiveCardWithEffect* card : _effect_cards )
        base_cost = card->ModifyCardCost( card, base_cost );

    return base_cost;
}

void Player::UseAction( decks::ActiveCardWithAction* card ) {
    if ( std::find( _action_cards.cbegin(), _action_cards.cend(), card ) == _action_cards.cend() )
        throw std::logic_error( "Player::UseAction: card was not played by this player!" );

    card->UseAction();
}

void Player::OnEffect( std::function<void( decks::ActiveCardWithEffect* )> effect ) {
    for ( decks::ActiveCardWithEffect* card : _effect_cards )
        effect( card );
}

void Player::SetOnDrawCardCallback( Callback<> callback ) { _on_draw_card.SetCallback( callback ); }
void Player::SetOnRaiseTRCallback( Callback<int> callback ) { _on_raise_tr.SetCallback( callback ); }
void Player::SetOnRaiseTemperatureCallback( Callback<> callback ) { _on_raise_temperature.SetCallback( callback ); }
void Player::SetOnPlaceOceanCallback( Callback<> callback ) { _on_place_ocean.SetCallback( callback ); }
void Player::SetOnPlaceOceanOnNonOceanCallback( Callback<> callback ) { _on_place_ocean_on_non_ocean.SetCallback( callback ); }
void Player::SetOnRaiseOxygenCallback( Callback<> callback ) { _on_raise_oxygen.SetCallback( callback ); }
void Player::SetOnPlaceGreeneryCallback( Callback<> callback ) { _on_place_greenery.SetCallback( callback ); }
void Player::SetOnPlaceGreeneryOnOceanCallback( Callback<> callback ) { _on_place_greenery_on_ocean.SetCallback( callback ); }
void Player::SetOnPlaceCityCallback( Callback<> callback ) { _on_place_city.SetCallback( callback ); }
void Player::SetOnPlaceNoctisCityCallback( Callback<> callback ) { _on_place_noctis_city.SetCallback( callback ); }
void Player::SetOnPlaceLonelyCityCallback( Callback<> callback ) { _on_place_lonely_city.SetCallback( callback ); }
void Player::SetOnPlaceUrbanizedAreaCallback( Callback<> callback ) { _on_place_urbanized_area.SetCallback( callback ); }
void Player::SetOnResourceAmountChangedCallback( Callback<Resource, int> callback ) { _on_resource_amount_changed.SetCallback( callback ); }
void Player::SetOnResourceProductionAmountChangedCallback( Callback<Resource, int> callback ) { _on_resource_production_amount_changed.SetCallback( callback ); }
void Player::SetOnDestroyResourceCallback( Callback<Resource, int> callback ) { _on_destroy_resource.SetCallback( callback ); }
void Player::SetOnDestroyResourceProductionCallback( Callback<Resource, int> callback ) { _on_destroy_resource_production.SetCallback( callback ); }
inline void Player::SetOnConfirmSteelPaymentCallback( Callback<int, std::function<void()>> callback ) { _on_confirm_steel_payment.SetCallback( callback ); }
inline void Player::SetOnConfirmTitaniumPaymentCallback( Callback<int, std::function<void()>> callback ) { _on_confirm_titanium_payment.SetCallback( callback ); }


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
