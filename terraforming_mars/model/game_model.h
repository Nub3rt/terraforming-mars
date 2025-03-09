#pragma once

#include "card.fwd.h"
#include "deck.fwd.h"
#include "game_model.fwd.h"
#include "game_model_state.fwd.h"
#include "player.fwd.h"

#include <functional>
#include <queue>
#include <random>
#include <utility>

#include "board.h"
#include "event.h"
#include "resource.h"
#include "tile_type.h"

namespace model
{
class GameModel
{
    friend class GameModelState;
    friend class IdleState;
    friend class PlacementConfirmationState;
    friend class PaymentConfirmationState;

protected:
    class Request;

public:
    using pii = std::pair<int, int>;

    template<typename... Args>
    using Callback = std::function<void( Args... )>;

    virtual ~GameModel();

    virtual void Initialize( board::Board* board, decks::Deck* deck );

    inline const board::Board* get_board() const;
    inline const Player* get_local_player() const;

    inline int Temperature() const;
    inline int OceanCount() const;
    inline int Oxygen() const;
    inline int CityCount() const;

    bool IsTilePlaceable( const Player* player ) const;
    bool IsCityPlaceable( const Player* player ) const;
    bool IsUrbanizedAreaPlaceable( const Player* player ) const;
    bool IsAvailableLonelyTile( const Player* player ) const;

    virtual bool AreGlobalParametersFulfilled() const;

    inline bool CanUsePowerPlantSP() const;
    inline bool CanUseAsteroidSP() const;
    inline bool CanUseAquiferSP() const;
    inline bool CanUseGreenerySP() const;
    inline bool CanUseCitySP() const;
    inline bool CanConvertPlantsToGreenery() const;
    inline bool CanConvertHeatToTemperature() const;

    inline bool InIdleState() const; // can play cards, can use actions;
    inline bool CanPlayCards() const;
    inline bool CanUseActions() const;

    inline void SellCardSP( decks::Card* card );
    inline void UsePowerPlantSP();
    inline void UseAsteroidSP();
    inline void UseAquiferSP();
    inline void UseGreenerySP();
    inline void UseCitySP();

    inline void PlayCard( decks::Card* card );
    inline void UseAction( decks::Card* card );
    inline void ConvertPlantsToGreenery();
    inline void ConvertHeatToTemperature();

    inline void TilePlacementConfirmed( int q, int r );

    inline void PaymentConfirmed( int credit, int resource );

    inline void EndTurn();


    inline void SetOnDrawCard( Callback<> callback );
    inline void SetOnRaiseTR( Callback<int> callback );
    inline void SetOnRaiseTemperature( Callback<> callback );
    inline void SetOnRaiseOxygen( Callback<> callback );
    inline void SetOnResourceAmountChanged( Callback <Resource, int> callback );
    inline void SetOnResourceProductionAmountChanged( Callback <Resource, int> callback );
    inline void SetOnConfirmPayment( Callback<int, Resource, int> callback );
    inline void SetOnConfirmPlacement( Callback <board::TileType, std::vector<pii>> callback );

    static const int STARTING_TEMPTERATURE = -30;
    static const int STARTING_OCEAN_COUNT = 0;
    static const int STARTING_OXYGEN_LEVEL = 0;
    static const int MAX_TEMPTERATURE = 8;
    static const int MAX_OCEAN_COUNT = 9;
    static const int MAX_OXYGEN_LEVEL = 14;

protected:
    GameModel( int seed );

    bool _initialized = false;

    int _temperature = 0;
    int _ocean_count = 0;
    int _oxygen_level = 0;

    GameModelState* _state = nullptr;

    Player* _local_player = nullptr;

    board::Board* _board = nullptr;

    decks::Deck* _deck = nullptr;

    std::queue<Request*> _queued_request;

    std::mt19937 _random;

    Event<> _on_draw_card;
    Event<int> _on_raise_tr;
    Event<> _on_raise_temperature;
    Event<> _on_raise_oxygen;
    Event<Resource, int> _on_resource_amount_changed;
    Event<Resource, int> _on_resource_production_amount_changed;

    Event<int, Resource, int> _on_confirm_payment;
    Event<board::TileType, std::vector<pii>> _on_confirm_placement;


    virtual Player* CreateLocalPlayer() = 0;

    void SubscribeCallbacksOnPlayer( Player* player );

    inline void Player_OnDrawCard( Player* player );
    inline void Player_OnRaiseTR( Player* player, int amount );
    inline void Player_OnRaiseTemperature( Player* player );
    inline void Player_OnPlaceOcean( Player* player );
    inline void Player_OnPlaceOceanOnNonOcean( Player* player );
    inline void Player_OnRaiseOxygen( Player* player );
    inline void Player_OnPlaceGreenery( Player* player );
    inline void Player_OnPlaceGreeneryOnOcean( Player* player );
    inline void Player_OnPlaceCity( Player* player );
    inline void Player_OnPlaceNoctisCity( Player* player );
    inline void Player_OnPlaceLonelyCity( Player* player );
    inline void Player_OnPlaceUrbanizedArea( Player* player );
    inline void Player_OnResourceAmountChanged( Player* player, Resource resource, int amount );
    inline void Player_OnResourceProductionAmountChanged( Player* player, Resource resource, int amount );
    inline void Player_OnDestroyResource( Player* player, Resource resource, int amount );
    inline void Player_OnDestroyResourceProduction( Player* player, Resource resource, int amount );
    inline void Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment );
    inline void Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment );

    class Request
    {
    public:
        Request() {}
        virtual ~Request() {}

        virtual void Perform( GameModelState* state ) = 0;
    };

    class PlacementRequest : public Request
    {
    public:
        PlacementRequest( board::TileType type, std::vector<pii> valid_positions );

        board::TileType type;
        std::vector<pii> valid_positions;

        void Perform( GameModelState* state ) override;
    };

    class PaymentRequest : public Request
    {
    public:
        PaymentRequest( int cost, Resource resource, int resource_value );

        int cost;
        Resource resource;
        int resource_value;

        void Perform( GameModelState* state ) override;
    };
};
}
