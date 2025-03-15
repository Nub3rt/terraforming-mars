#pragma once

#include "card.fwd.h"
#include "deck.fwd.h"
#include "game_model.fwd.h"
#include "game_model_state.fwd.h"
#include "player.fwd.h"

#include <array>
#include <functional>
#include <queue>
#include <random>
#include <utility>

#include "active_card_with_action.h"
#include "board.h"
#include "constants.h"
#include "event.h"
#include "resource.h"
#include "tile_type.h"

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
    using pii = std::pair<int, int>;

    template<typename... Args>
    using Callback = std::function<void( Args... )>;

    virtual ~GameModel();

    virtual void Initialize( board::Board* board, decks::Deck* deck );
    virtual void Start();

    inline int get_generation() const;
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
    inline void ConvertPlantsToGreenery();
    inline void ConvertHeatToTemperature();

    inline void PlayCard( decks::Card* card );
    inline void UseAction( decks::ActiveCardWithAction* card );

    // Research State
    inline void ToggleToBuyCard( int index );
    inline int GetTotalCost() const;
    inline void ConfirmPurchases();

    // Placement Confirmation State
    inline void TilePlacementConfirmed( int q, int r );

    // Payment Confirmation State
    inline void PaymentConfirmed( int credit, int resource );

    inline void EndTurn();


    inline void SetOnDrawCard( Callback<decks::Card*> callback );
    inline void SetOnPlayCard( Callback<decks::Card*> callback );
    inline void SetOnRaiseTR( Callback<int> callback );
    inline void SetOnRaiseTemperature( Callback<> callback );
    inline void SetOnRaiseOxygen( Callback<> callback );
    inline void SetOnPlaceTile( Callback<pii> callback );
    inline void SetOnResourceAmountChanged( Callback<Resource, int> callback );
    inline void SetOnResourceProductionAmountChanged( Callback<Resource, int> callback );
    inline void SetOnResearchConfirmed( Callback<std::array<bool, RESEARCH_CARD_NUM>> callback );
    inline void SetOnConfirmResearch( Callback<std::array<decks::Card*, RESEARCH_CARD_NUM>> callback );
    inline void SetOnConfirmPayment( Callback<int, Resource, int> callback );
    inline void SetOnConfirmPlacement( Callback<board::TileType, std::vector<pii>> callback );
    inline void SetOnConfirmDestroyResource( Callback<Resource, int> callback );
    inline void SetOnConfirmDestroyResourceProduction( Callback<Resource, int> callback );
    inline void SetOnGameEnd( Callback<> callback );

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

    Player* _local_player = nullptr;

    board::Board* _board = nullptr;

    decks::Deck* _deck = nullptr;

    std::queue<Request*> _queued_request;

    std::mt19937 _random;

    Event<decks::Card*> _on_draw_card;
    Event<decks::Card*> _on_play_card;
    Event<int> _on_raise_tr;
    Event<> _on_raise_temperature;
    Event<> _on_raise_oxygen;
    Event<pii> _on_place_tile;
    Event<Resource, int> _on_resource_amount_changed;
    Event<Resource, int> _on_resource_production_amount_changed;
    Event<std::array<bool, RESEARCH_CARD_NUM>> _on_research_confirmed;

    Event<std::array<decks::Card*, RESEARCH_CARD_NUM>> _on_confirm_research;
    Event<int, Resource, int> _on_confirm_payment;
    Event<board::TileType, std::vector<pii>> _on_confirm_placement;
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

    void ChangeState( GameModelState* state );

    virtual void EndGame();

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
    virtual void Player_OnDestroyResource( Player* player, Resource resource, int amount );
    virtual void Player_OnDestroyResourceProduction( Player* player, Resource resource, int amount );
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
        PostLastGenerationGreeneryPlacementRequest( std::vector<pii> valid_positions );

        std::vector<pii> valid_positions;

        void Perform( GameModelState* state ) override;
    };
};
}
