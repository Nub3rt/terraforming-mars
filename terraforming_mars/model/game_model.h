#pragma once

#include "card.fwd.h"
#include "deck.fwd.h"
#include "game_model.fwd.h"
#include "game_model_state.fwd.h"
#include "player.fwd.h"

#include <functional>
#include <queue>
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

class Request;

public:
    using pii = std::pair<int, int>;

    template<typename... Args>
    using Callback = std::function<void( Args... )>;

    virtual ~GameModel();

    virtual void Initialize( board::Board* board, decks::Deck* deck );

    inline const board::Board* get_board() const;
    inline const Player* get_local_player() const;

    int Temperature() const;
    int OceanCount() const;
    int Oxygen() const;
    int CityCount() const;

    bool IsTilePlaceable() const;
    bool IsCityPlaceable() const;
    bool IsUrbanizedAreaPlaceable() const;
    bool IsAvailableLonelyTile() const;

    bool AreGlobalParametersFulfilled() const;

    bool CanUsePowerPlantSP() const;
    bool CanUseAsteroidSP() const;
    bool CanUseAquiferSP() const;
    bool CanUseGreenerySP() const;
    bool CanUseCitySP() const;

    void SellCardSP( decks::Card* card );
    void UsePowerPlantSP();
    void UseAsteroidSP();
    void UseAquiferSP();
    void UseGreenerySP();
    void UseCitySP();

    void PlayCard( decks::Card* card );
    void UseAction( decks::Card* card );
    void ConvertPlantsToGreenery();
    void ConvertHeatToTemperature();

    void TilePlacementConfirmed( int q, int r );

    void PaymentConfirmed( int credit, int resource );

    void EndGeneration();


    inline void SetOnDrawCard( Callback<> callback );
    inline void SetOnRaiseTR( Callback<int> callback );
    inline void SetOnRaiseTemperature( Callback<> callback );
    inline void SetOnRaiseOxygen( Callback<> callback );
    inline void SetOnResourceAmountChanged( Callback <Resource, int> callback );
    inline void SetOnResourceProductionAmountChanged( Callback <Resource, int> callback );

    inline void SetOnConfirmPayment( Callback<int, Resource, int> callback );
    inline void SetOnConfirmPlacement( Callback <board::TileType, const std::vector<pii>> callback );

protected:
    GameModel();

    bool _initialized = false;

    int _temperature = 0;
    int _ocean_count = 0;
    int _oxygen_level = 0;

    GameModelState* _state = nullptr;

    Player* _local_player = nullptr;

    board::Board* _board = nullptr;

    decks::Deck* _deck = nullptr;


    Event<> _on_draw_card;
    Event<int> _on_raise_tr;
    Event<> _on_raise_temperature;
    Event<> _on_raise_oxygen;
    Event<Resource, int> _on_resource_amount_changed;
    Event<Resource, int> _on_resource_production_amount_changed;

    Event<int, Resource, int> _on_confirm_payment;
    Event<board::TileType, const std::vector<pii>> _on_confirm_placement;

    std::queue<Request*> _queued_request;

    virtual Player* CreateLocalPlayer() = 0;

    void SubscribeCallbacksOnPlayer( Player* player );

    void OnDrawCard( Player* player );
    void OnRaiseTR( Player* player, int amount );
    void OnRaiseTemperature( Player* player );
    void OnPlaceOcean( Player* player );
    void OnPlaceOceanOnNonOcean( Player* player );
    void OnRaiseOxygen( Player* player );
    void OnPlaceGreenery( Player* player );
    void OnPlaceGreeneryOnOcean( Player* player );
    void OnPlaceCity( Player* player );
    void OnPlaceNoctisCity( Player* player );
    void OnPlaceLonelyCity( Player* player );
    void OnPlaceUrbanizedArea( Player* player );
    void OnResourceAmountChanged( Player* player, Resource resource, int amount );
    void OnResourceProductionAmountChanged( Player* player, Resource resource, int amount );
    void OnDestroyResource( Player* player, Resource resource, int amount );
    void OnDestroyResourceProduction( Player* player, Resource resource, int amount );

    static const int STARTING_TEMPTERATURE = -30;
    static const int STARTING_OCEAN_COUNT = 0;
    static const int STARTING_OXYGEN_LEVEL = 0;
    static const int MAX_TEMPTERATURE = 8;
    static const int MAX_OCEAN_COUNT = 9;
    static const int MAX_OXYGEN_LEVEL = 14;

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

        const board::TileType type;
        const std::vector<pii> valid_positions;

        void Perform( GameModelState* state ) override;
    };

    class PaymentRequest : public Request
    {
    public:
        PaymentRequest( int cost, Resource resource, int resource_value );

        const int cost;
        const Resource resource;
        const int resource_value;

        void Perform( GameModelState* state ) override;
    };
};
}
