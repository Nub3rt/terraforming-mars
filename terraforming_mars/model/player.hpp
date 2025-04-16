#pragma once

#include "decks/card.fwd.hpp"
#include "player.fwd.hpp"

#include <array>
#include <functional>
#include <vector>

#include "decks/active_card.hpp"
#include "decks/active_card_with_action.hpp"
#include "decks/active_card_with_effect.hpp"
#include "decks/automated_card.hpp"
#include "constants.hpp"
#include "event.hpp"
#include "decks/event_card.hpp"
#include "resource.hpp"
#include "tag.hpp"

namespace model
{
class Player
{
public:
    template<typename... Args>
    using CallbackPp = std::function<void( Player*, Args... )>;

    Player( int starting_tr, int starting_resource_production_amount );
    ~Player();
    Player( const Player& other ) = delete;
    Player( Player&& other ) = delete;
    Player& operator=( const Player& other ) = delete;
    Player& operator=( Player&& other ) = delete;

    inline const int get_tr() const noexcept { return _tr; }

    inline const std::array<int, +Resource::MAX + 1>& get_resources() const noexcept { return _resources; }
    inline const std::array<int, +Resource::MAX + 1>& get_resource_productions() const noexcept { return _resource_productions; }

    inline int get_steel_value() const noexcept { return _steel_value; }
    inline int get_titanium_value() const noexcept { return _titanium_value; }

    inline int get_greenery_cost() const noexcept { return _greenery_cost; }
    inline int get_temperature_cost() const noexcept { return _temperature_cost; }

    inline const std::vector<const decks::Card*>& get_hand() const noexcept {
        return reinterpret_cast<const std::vector<const decks::Card*>&>( _hand );
    }
    inline const std::vector<const decks::EventCard*>& get_event_cards() const noexcept {
        return reinterpret_cast<const std::vector<const decks::EventCard*>&>( _event_cards );
    }
    inline const std::vector<const decks::AutomatedCard*>& get_automated_cards() const noexcept {
        return reinterpret_cast<const std::vector<const decks::AutomatedCard*>&>( _automated_cards );
    }
    inline const std::vector<const decks::ActiveCardWithAction*>& get_action_cards() const noexcept {
        return reinterpret_cast<const std::vector<const decks::ActiveCardWithAction*>&>( _action_cards );
    }
    inline const std::vector<const decks::ActiveCardWithEffect*>& get_effect_cards() const noexcept {
        return reinterpret_cast<const std::vector<const decks::ActiveCardWithEffect*>&>( _effect_cards );
    }

    inline void DrawCard() { _on_draw_card.Invoke( this ); }
    inline void RaiseTR( int amount ) { _on_raise_tr.Invoke( this, amount ); }
    inline void RaiseTemperature() { _on_raise_temperature.Invoke( this ); }
    inline void PlaceOcean() { _on_place_ocean.Invoke( this ); }
    inline void PlaceOceanOnNonOcean() { _on_place_ocean_on_non_ocean.Invoke( this ); }
    inline void RaiseOxygen() { _on_raise_oxygen.Invoke( this ); }
    inline void PlaceGreenery() { _on_place_greenery.Invoke( this ); }
    inline void PlaceGreeneryOnOcean() { _on_place_greenery_on_ocean.Invoke( this ); }
    inline void PlaceCity() { _on_place_city.Invoke( this ); }
    inline void PlaceNoctisCity() { _on_place_noctis_city.Invoke( this ); }
    inline void PlaceLonelyCity() { _on_place_lonely_city.Invoke( this ); }
    inline void PlaceUrbanizedArea() { _on_place_urbanized_area.Invoke( this ); }

    void GetTR( int amount );

    int GetResource( Resource resource ) const;
    int GetResourceProduction( Resource resource ) const;

    void GainResource( Resource resource, int amount );
    void GainResourceProduction( Resource resource, int amount );

    void LoseResource( Resource resource, int amount );
    void LoseResourceProduction( Resource resource, int amount );

    void DestroyResource( Resource resource, int amount );
    void DestroyResourceProduction( Resource resource, int amount );

    // void AddResouce( ... );

    void PerformProductionPhase();


    int GetTagCount( Tag tag );

    int GetMaxPayAmountForBuilding() const;
    int GetMaxPayAmountForSpace() const;
    void ConfirmSteelPayment( int cost, std::function<void()> after_payment );
    void ConfirmTitaniumPayment( int cost, std::function<void()> after_payment );

    void GetCard( decks::Card* card );
    void SellCard( const decks::Card* card );
    void PlayCard( const decks::Card* card );
    int CalculateCardCost( const decks::Card* card, int base_cost ) const;

    void UseAction( const decks::ActiveCardWithAction* card );
    void OnEffect( std::function<void( decks::ActiveCardWithEffect* )> effect );


    inline void SetOnDrawCardCallback( CallbackPp<> callback ) { _on_draw_card.SetCallback( callback ); }
    inline void SetOnRaiseTRCallback( CallbackPp<int> callback ) { _on_raise_tr.SetCallback( callback ); }

    inline void SetOnRaiseTemperatureCallback( CallbackPp<> callback ) { _on_raise_temperature.SetCallback( callback ); }
    inline void SetOnPlaceOceanCallback( CallbackPp<> callback ) { _on_place_ocean.SetCallback( callback ); }
    inline void SetOnPlaceOceanOnNonOceanCallback( CallbackPp<> callback ) { _on_place_ocean_on_non_ocean.SetCallback( callback ); }
    inline void SetOnRaiseOxygenCallback( CallbackPp<> callback ) { _on_raise_oxygen.SetCallback( callback ); }

    inline void SetOnPlaceGreeneryCallback( CallbackPp<> callback ) { _on_place_greenery.SetCallback( callback ); }
    inline void SetOnPlaceGreeneryOnOceanCallback( CallbackPp<> callback ) { _on_place_greenery_on_ocean.SetCallback( callback ); }
    inline void SetOnPlaceCityCallback( CallbackPp<> callback ) { _on_place_city.SetCallback( callback ); }
    inline void SetOnPlaceNoctisCityCallback( CallbackPp<> callback ) { _on_place_noctis_city.SetCallback( callback ); }
    inline void SetOnPlaceLonelyCityCallback( CallbackPp<> callback ) { _on_place_lonely_city.SetCallback( callback ); }
    inline void SetOnPlaceUrbanizedAreaCallback( CallbackPp<> callback ) { _on_place_urbanized_area.SetCallback( callback ); }

    inline void SetOnResourceAmountChangedCallback( CallbackPp<Resource, int> callback ) { _on_resource_amount_changed.SetCallback( callback ); }
    inline void SetOnResourceProductionAmountChangedCallback( CallbackPp<Resource, int> callback ) { _on_resource_production_amount_changed.SetCallback( callback ); }
    inline void SetOnDestroyResourceCallback( CallbackPp<Resource, int> callback ) { _on_destroy_resource.SetCallback( callback ); }
    inline void SetOnDestroyResourceProductionCallback( CallbackPp<Resource, int> callback ) { _on_destroy_resource_production.SetCallback( callback ); }

    inline void SetOnConfirmSteelPaymentCallback( CallbackPp <int, std::function<void()>> callback ) { _on_confirm_steel_payment.SetCallback( callback ); }
    inline void SetOnConfirmTitaniumPaymentCallback( CallbackPp <int, std::function<void()>> callback ) { _on_confirm_titanium_payment.SetCallback( callback ); }

private:
    int _tr;

    std::array<int, +Tag::MAX + 1> _tags;
    std::array<int, +Resource::MAX + 1> _resources;
    std::array<int, +Resource::MAX + 1> _resource_productions;

    std::vector<decks::Card*> _hand;
    std::vector<decks::EventCard*> _event_cards;
    std::vector<decks::AutomatedCard*> _automated_cards;
    std::vector<decks::ActiveCard*> _active_cards;
    std::vector<decks::ActiveCardWithAction*> _action_cards;
    std::vector<decks::ActiveCardWithEffect*> _effect_cards;

    int _steel_value = STARTING_STEEL_VALUE;
    int _titanium_value = STARTING_TITANIUM_VALUE;

    int _greenery_cost = BASE_GREENERY_COST;
    int _temperature_cost = BASE_TEMPERATURE_COST;

    Event<Player*> _on_draw_card;
    Event<Player*, int> _on_raise_tr;
    Event<Player*> _on_raise_temperature;
    Event<Player*> _on_place_ocean;
    Event<Player*> _on_place_ocean_on_non_ocean;
    Event<Player*> _on_raise_oxygen;
    Event<Player*> _on_place_greenery;
    Event<Player*> _on_place_greenery_on_ocean;
    Event<Player*> _on_place_city;
    Event<Player*> _on_place_noctis_city;
    Event<Player*> _on_place_lonely_city;
    Event<Player*> _on_place_urbanized_area;
    Event<Player*, Resource, int> _on_resource_amount_changed;
    Event<Player*, Resource, int> _on_resource_production_amount_changed;
    Event<Player*, Resource, int> _on_destroy_resource;
    Event<Player*, Resource, int> _on_destroy_resource_production;
    Event<Player*, int, std::function<void()>> _on_confirm_steel_payment;
    Event<Player*, int, std::function<void()>> _on_confirm_titanium_payment;

    void DoPlayCard( decks::Card* card );
};
}
