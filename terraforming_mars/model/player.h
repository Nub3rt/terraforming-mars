#pragma once

#include "card.fwd.h"
#include "player.fwd.h"

#include <array>
#include <functional>
#include <vector>

#include "active_card.h"
#include "active_card_with_action.h"
#include "active_card_with_effect.h"
#include "automated_card.h"
#include "event.h"
#include "event_card.h"
#include "resource.h"
#include "tag.h"

namespace model
{
class Player
{
public:
    template<typename... Args>
    using Callback = std::function<void( Player*, Args... )>;

    Player( int starting_resource_production_amount );
    ~Player();
    Player( const Player& other ) = delete;
    Player( Player&& other ) = delete;
    Player& operator=( const Player& other ) = delete;
    Player& operator=( Player&& other ) = delete;

    inline const auto& get_resources() const noexcept;
    inline const auto& get_resource_productions() const noexcept;

    inline int get_steel_value() const noexcept;
    inline int get_titanium_value() const noexcept;

    inline const std::vector<decks::Card*>& get_hand() const noexcept;
    inline const std::vector<decks::EventCard*>& get_event_cards() const noexcept;
    inline const std::vector<decks::AutomatedCard*>& get_automated_cards() const noexcept;
    inline const std::vector<decks::ActiveCardWithAction*>& get_action_cards() const noexcept;
    inline const std::vector<decks::ActiveCardWithEffect*>& get_effect_cards() const noexcept;

    inline void DrawCard();

    inline void RaiseTR( int amount );

    inline void RaiseTemperature();
    inline void PlaceOcean();
    inline void PlaceOceanOnNonOcean();
    inline void RaiseOxygen();

    inline void PlaceGreenery();
    inline void PlaceGreeneryOnOcean();
    inline void PlaceCity();
    inline void PlaceNoctisCity();
    inline void PlaceLonelyCity();
    inline void PlaceUrbanizedArea();

    inline int GetResource( Resource resource ) const;
    inline int GetResourceProduction( Resource resource ) const;

    inline void GainResource( Resource resource, int amount );
    inline void GainResourceProduction( Resource resource, int amount );

    inline void LoseResource( Resource resource, int amount );
    inline void LoseResourceProduction( Resource resource, int amount );

    inline void DestroyResource( Resource resource, int amount );
    inline void DestroyResourceProduction( Resource resource, int amount );

    // void AddResouce( ... );


    int GetTagCount( Tag tag );

    int GetMaxPayAmountForBuilding() const;
    int GetMaxPayAmountForSpace() const;
    void ConfirmSteelPayment( int cost, std::function<void()> after_payment );
    void ConfirmTitaniumPayment( int cost, std::function<void()> after_payment );

    void GetCard( decks::Card* card );
    void DiscardCard( decks::Card* card );
    void PlayCard( decks::Card* card );
    int CalculateCardCost( const decks::Card* card, int base_cost ) const;

    void OnEffect( std::function<void( decks::ActiveCardWithEffect* )> effect );


    inline void SetOnDrawCardCallback( Callback<> callback );

    inline void SetOnRaiseTRCallback( Callback<int> callback );

    inline void SetOnRaiseTemperatureCallback( Callback<> callback );
    inline void SetOnPlaceOceanCallback( Callback<> callback );
    inline void SetOnPlaceOceanOnNonOceanCallback( Callback<> callback );
    inline void SetOnRaiseOxygenCallback( Callback<> callback );

    inline void SetOnPlaceGreeneryCallback( Callback<> callback );
    inline void SetOnPlaceGreeneryOnOceanCallback( Callback<> callback );
    inline void SetOnPlaceCityCallback( Callback<> callback );
    inline void SetOnPlaceNoctisCityCallback( Callback<> callback );
    inline void SetOnPlaceLonelyCityCallback( Callback<> callback );
    inline void SetOnPlaceUrbanizedAreaCallback( Callback<> callback );

    inline void SetOnResourceAmountChangedCallback( Callback<Resource, int> callback );
    inline void SetOnResourceProductionAmountChangedCallback( Callback<Resource, int> callback );
    inline void SetOnDestroyResourceCallback( Callback<Resource, int> callback );
    inline void SetOnDestroyResourceProductionCallback( Callback<Resource, int> callback );

private:
    std::array<int, +Tag::MAX + 1> _tags;
    std::array<int, +Resource::MAX + 1> _resources;
    std::array<int, +Resource::MAX + 1> _resource_productions;

    std::vector<decks::Card*> _hand;
    std::vector<decks::EventCard*> _event_cards;
    std::vector<decks::AutomatedCard*> _automated_cards;
    std::vector<decks::ActiveCard*> _active_cards;
    std::vector<decks::ActiveCardWithAction*> _action_cards;
    std::vector<decks::ActiveCardWithEffect*> _effect_cards;

    int _steel_value = 2;
    int _titanium_value = 3;

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
