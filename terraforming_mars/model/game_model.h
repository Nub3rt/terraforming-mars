#pragma once

#include "decks/card.fwd.h"
#include "decks/deck.fwd.h"
#include "game_model.fwd.h"
#include "game_model_state.fwd.h"
#include "player.fwd.h"

#include <array>
#include <functional>
#include <queue>
#include <random>
#include <utility>

#include "decks/active_card_with_action.h"
#include "boards/board.h"
#include "constants.h"
#include "event.h"
#include "resource.h"
#include "boards/tile_type.h"

namespace model
{
class GameModel
{
    friend class GameModelState;

    friend class IdleState;
    friend class ResearchState;
    friend class PlacementConfirmationState;
    friend class PaymentConfirmationState;
    friend class PostLastGenerationState;
    friend class PostLastGenerationPlacementConfirmationState;
    friend class GameOverState;

    friend class SoloIdleState;
    friend class SoloPostLastGenerationState;

protected:
    class Request;
    class PlacementRequest;
    class PaymentRequest;
    class PostLastGenerationGreeneryPlacementRequest;

public:
    template<typename... Args>
    using Callback = std::function<void( Args... )>;

    virtual ~GameModel();

    virtual void Initialize( boards::Board* board, decks::Deck* deck );
    virtual void Start();
    virtual void Update();

    inline int get_generation() const { return _generation; }
    inline const boards::Board* get_board() const { return _board; }
    inline const Player* get_local_player() const { return _local_player; }

    inline int Temperature() const { return _temperature; }
    inline int OceanCount() const { return _ocean_count; }
    inline int Oxygen() const { return _oxygen_level; }
    inline int CityCount() const { return static_cast<int>( _board->GetTilesOfType( boards::TileType::CITY ).size() ); }

    bool IsTilePlaceable( const Player* player ) const;
    bool IsCityPlaceable( const Player* player ) const;
    bool IsUrbanizedAreaPlaceable( const Player* player ) const;
    bool IsAvailableLonelyTile( const Player* player ) const;

    virtual bool AreGlobalParametersFulfilled() const;

    bool CanUsePowerPlantSP() const;
    bool CanUseAsteroidSP() const;
    bool CanUseAquiferSP() const;
    bool CanUseGreenerySP() const;
    bool CanUseCitySP() const;
    bool CanConvertPlantsToGreenery() const;
    bool CanConvertHeatToTemperature() const;

    bool InIdleState() const; // can play cards, can use actions;
    bool CanPlayCards() const;
    bool CanUseActions() const;

    void SellCardSP( const decks::Card* card );
    void UsePowerPlantSP();
    void UseAsteroidSP();
    void UseAquiferSP();
    void UseGreenerySP();
    void UseCitySP();
    void ConvertPlantsToGreenery();
    void ConvertHeatToTemperature();

    void PlayCard( const decks::Card* card );
    void UseAction( const decks::ActiveCardWithAction* card );

    // Research State
    void ToggleToBuyCard( int index );
    int GetTotalCost() const;
    void ConfirmPurchases();

    // Placement Confirmation State
    void TilePlacementConfirmed( int q, int r );

    // Payment Confirmation State
    void PaymentConfirmed( int credit, int resource );

    void EndTurn();


    inline void SetOnDrawCard( Callback<const decks::Card*> callback ) { _on_draw_card.SetCallback( callback ); }
    inline void SetOnPlayCard( Callback<const decks::Card*> callback ) { _on_play_card.SetCallback( callback ); }
    inline void SetOnRaiseTR( Callback<int> callback ) { _on_raise_tr.SetCallback( callback ); }
    inline void SetOnRaiseTemperature( Callback<> callback ) { _on_raise_temperature.SetCallback( callback ); }
    inline void SetOnRaiseOxygen( Callback<> callback ) { _on_raise_oxygen.SetCallback( callback ); }
    inline void SetOnPlaceTile( Callback<std::pair<int, int>> callback ) { _on_place_tile.SetCallback( callback ); }
    inline void SetOnResourceAmountChanged( Callback<Resource, int> callback ) { _on_resource_amount_changed.SetCallback( callback ); }
    inline void SetOnResourceProductionAmountChanged( Callback<Resource, int> callback ) { _on_resource_production_amount_changed.SetCallback( callback ); }
    inline void SetOnResearchConfirmed( Callback<std::array<bool, RESEARCH_CARD_NUM>> callback ) { _on_research_confirmed.SetCallback( callback ); }
    inline void SetOnConfirmResearch( Callback<std::array<const decks::Card*, RESEARCH_CARD_NUM>> callback ) { _on_confirm_research.SetCallback( callback ); }
    inline void SetOnConfirmPayment( Callback<int, Resource, int> callback ) { _on_confirm_payment.SetCallback( callback ); }
    inline void SetOnConfirmPlacement( Callback<boards::TileType, std::vector<std::pair<int, int>>> callback ) { _on_confirm_placement.SetCallback( callback ); }
    inline void SetOnConfirmDestroyResource( Callback<Resource, int> callback ) { _on_confirm_destroy_resource.SetCallback( callback ); }
    inline void SetOnConfirmDestroyResourceProduction( Callback<Resource, int> callback ) { _on_confirm_destroy_resource_production.SetCallback( callback ); }
    inline void SetOnGameEnd( Callback<> callback ) { _on_game_end.SetCallback( callback ); }

protected:
    GameModel( int seed );

    bool _initialized = false;
    bool _started = false;

    bool _in_post_last_generation = false;
    bool _game_ended = false;

    int _generation = 0;
    int _temperature = 0;
    int _ocean_count = 0;
    int _oxygen_level = 0;

    GameModelState* _state = nullptr;
    GameModelState* _next_state = nullptr;

    Player* _local_player = nullptr;

    boards::Board* _board = nullptr;

    decks::Deck* _deck = nullptr;

    std::queue<Request*> _queued_request;

    std::mt19937 _random;

    Event<const decks::Card*> _on_draw_card;
    Event<const decks::Card*> _on_play_card;
    Event<int> _on_raise_tr;
    Event<> _on_raise_temperature;
    Event<> _on_raise_oxygen;
    Event<std::pair<int, int>> _on_place_tile;
    Event<Resource, int> _on_resource_amount_changed;
    Event<Resource, int> _on_resource_production_amount_changed;
    Event<std::array<bool, RESEARCH_CARD_NUM>> _on_research_confirmed;

    Event<std::array<const decks::Card*, RESEARCH_CARD_NUM>> _on_confirm_research;
    Event<int, Resource, int> _on_confirm_payment;
    Event<boards::TileType, std::vector<std::pair<int, int>>> _on_confirm_placement;
    Event<Resource, int> _on_confirm_destroy_resource;
    Event<Resource, int> _on_confirm_destroy_resource_production;

    Event<> _on_game_end;


    virtual Player* CreateLocalPlayer() = 0;

    virtual void SubscribeCallbacksOnPlayer( Player* player );

    virtual IdleState* CreateIdleState();
    virtual ResearchState* CreateResearchState();
    virtual PlacementConfirmationState* CreatePlacementConfirmationState( PlacementRequest* request );
    virtual PaymentConfirmationState* CreatePaymentConfirmationState( PaymentRequest* request );
    virtual PostLastGenerationState* CreatePostLastGenerationState();
    virtual PostLastGenerationPlacementConfirmationState* CreatePostLastGenerationPlacementConfirmationState( PostLastGenerationGreeneryPlacementRequest* request );
    virtual GameOverState* CreateGameOverState();

    void RequestStateChange( GameModelState* state );

    virtual void EndGame();

    virtual void Player_OnDrawCard( Player* player );
    virtual void Player_OnRaiseTR( Player* player, int amount );
    virtual void Player_OnRaiseTemperature( Player* player );
    virtual void Player_OnPlaceOcean( Player* player );
    virtual void Player_OnPlaceOceanOnNonOcean( Player* player );
    virtual void Player_OnRaiseOxygen( Player* player );
    virtual void Player_OnPlaceGreenery( Player* player );
    virtual void Player_OnPlaceGreeneryOnOcean( Player* player );
    virtual void Player_OnPlaceCity( Player* player );
    virtual void Player_OnPlaceNoctisCity( Player* player );
    virtual void Player_OnPlaceLonelyCity( Player* player );
    virtual void Player_OnPlaceUrbanizedArea( Player* player );
    virtual void Player_OnResourceAmountChanged( Player* player, Resource resource, int amount );
    virtual void Player_OnResourceProductionAmountChanged( Player* player, Resource resource, int amount );
    virtual void Player_OnDestroyResource( Player* player, Resource resource, int amount );
    virtual void Player_OnDestroyResourceProduction( Player* player, Resource resource, int amount );
    virtual void Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment );
    virtual void Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment );

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
        PlacementRequest( boards::TileType type, std::vector<std::pair<int, int>> valid_positions );

        boards::TileType type;
        std::vector<std::pair<int, int>> valid_positions;

        void Perform( GameModelState* state ) override;
    };

    class PaymentRequest : public Request
    {
    public:
        PaymentRequest( int cost, Resource resource, int resource_value, std::function<void()> after_payment );

        int cost;
        Resource resource;
        int resource_value;
        std::function<void()> after_payment;

        void Perform( GameModelState* state ) override;
    };

    class PostLastGenerationGreeneryPlacementRequest : public Request
    {
    public:
        PostLastGenerationGreeneryPlacementRequest( std::vector<std::pair<int, int>> valid_positions );

        std::vector<std::pair<int, int>> valid_positions;

        void Perform( GameModelState* state ) override;
    };
};
}
