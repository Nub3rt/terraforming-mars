#include "player.h"

#include <algorithm>
#include <array>
#include <functional>
#include <stdexcept>
#include <vector>

#include "card.h"
#include "active_card.h"
#include "active_card_with_action.h"
#include "active_card_with_effect.h"
#include "automated_card.h"
#include "event_card.h"
#include "resource.h"
#include "tag.h"

namespace model
{
Player::Player( int starting_resource_production_amount ) : _tags(), _resources(), _resource_productions() {
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

inline const auto& Player::get_resources() const noexcept { return _resources; }
inline const auto& Player::get_resource_productions() const noexcept { return _resource_productions; }
inline int Player::get_steel_value() const noexcept { return _steel_value; }
inline int Player::get_titanium_value() const noexcept { return _titanium_value; }
inline const std::vector<decks::Card*>& Player::get_hand() const noexcept { return _hand; }
inline const std::vector<decks::EventCard*>& Player::get_event_cards() const noexcept { return _event_cards; }
inline const std::vector<decks::AutomatedCard*>& Player::get_automated_cards() const noexcept { return _automated_cards; }
inline const std::vector<decks::ActiveCardWithAction*>& Player::get_action_cards() const noexcept { return _action_cards; }
inline const std::vector<decks::ActiveCardWithEffect*>& Player::get_effect_cards() const noexcept { return _effect_cards; }

void Player::DrawCard() { _on_draw_card.Trigger( this ); }
void Player::RaiseTR( int amount ) { _on_raise_tr.Trigger( this, amount ); }
void Player::RaiseTemperature() { _on_raise_temperature.Trigger( this ); }
void Player::PlaceOcean() { _on_place_ocean.Trigger( this ); }
void Player::PlaceOceanOnNonOcean() { _on_place_ocean_on_non_ocean.Trigger( this ); }
void Player::RaiseOxygen() { _on_raise_oxygen.Trigger( this ); }
void Player::PlaceGreenery() { _on_place_greenery.Trigger( this ); }
void Player::PlaceGreeneryOnOcean() { _on_place_greenery_on_ocean.Trigger( this ); }
void Player::PlaceCity() { _on_place_city.Trigger( this ); }
void Player::PlaceNoctisCity() { _on_place_noctis_city.Trigger( this ); }
void Player::PlaceLonelyCity() { _on_place_lonely_city.Trigger( this ); }
void Player::PlaceUrbanizedArea() { _on_place_urbanized_area.Trigger( this ); }

int Player::GetResource( Resource resource ) const { return _resources[ +resource ]; }
int Player::GetResourceProduction( Resource resource ) const { return _resource_productions[ +resource ]; }

void Player::GainResource( Resource resource, int amount ) {
    _resources[ +resource ] += amount;
    _on_resource_amount_changed.Trigger( this, resource, amount );
}
void Player::GainResourceProduction( Resource resource, int amount ) {
    _resource_productions[ +resource ] += amount;
    _on_resource_production_amount_changed.Trigger( this, resource, amount );
}
void Player::LoseResource( Resource resource, int amount ) {
    _resources[ +resource ] -= amount;
    _on_resource_amount_changed.Trigger( this, resource, -amount );
}
void Player::LoseResourceProduction( Resource resource, int amount ) {
    _resource_productions[ +resource ] -= amount;
    _on_resource_production_amount_changed.Trigger( this, resource, -amount );
}

void Player::DestroyResource( Resource resource, int amount ) { _on_destroy_resource.Trigger( this, resource, amount ); }
void Player::DestroyResourceProduction( Resource resource, int amount ) { _on_destroy_resource_production.Trigger( this, resource, amount ); }

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

    _on_confirm_steel_payment.Trigger( this, cost, after_payment );
}

void Player::ConfirmTitaniumPayment( int cost, std::function<void()> after_payment ) {
    if ( _resources[ +Resource::TITANIUM ] == 0 ) {
        LoseResource( Resource::TITANIUM, cost );
        after_payment();
        return;
    }

    _on_confirm_titanium_payment.Trigger( this, cost, after_payment );
}

void Player::GetCard( decks::Card* card ) {
    _hand.push_back( card );
}

void Player::DiscardCard( decks::Card* card ) {
    auto it_to_card = std::find( _hand.cbegin(), _hand.cend(), card );

    if ( it_to_card == _hand.cend() )
        throw std::logic_error( "Player::DiscardCard: card was not in hand!" );

    _hand.erase( it_to_card );
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
    for (auto effect_card : _effect_cards )
        base_cost = effect_card->ModifyCardCost( card, base_cost );

    return base_cost;
}

void Player::OnEffect( std::function<void( decks::ActiveCardWithEffect* )> effect ) {
    for ( auto card : _effect_cards )
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
