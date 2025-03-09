#include "game_model.h"

#include <stdexcept>

#include "board.h"
#include "deck.h"
#include "event.h"
#include "game_model_state.h"
#include "player.h"

namespace model
{
template<typename... Args>
using Callback = std::function<void( Args... )>;

model::GameModel::GameModel( int seed ) : _random( seed ) {}

GameModel::~GameModel() {
    delete _board;
    delete _deck;
    delete _local_player;
}

void GameModel::Initialize( board::Board* board, decks::Deck* deck ) {
    if ( _initialized )
        throw std::logic_error( "GameModel::Initialize: model was already initialized!" );

    _initialized = true;

    _state = new IdleState( this );

    _temperature = STARTING_TEMPTERATURE;
    _ocean_count = STARTING_OCEAN_COUNT;
    _oxygen_level = STARTING_OXYGEN_LEVEL;

    _board = board;
    _deck = deck;

    _local_player = CreateLocalPlayer();
    SubscribeCallbacksOnPlayer( _local_player );
}

inline const board::Board* GameModel::get_board() const { return _board; }
inline const Player* GameModel::get_local_player() const { return _local_player; }

inline int GameModel::Temperature() const { return _temperature; }
inline int GameModel::OceanCount() const { return _ocean_count; }
inline int GameModel::Oxygen() const { return _oxygen_level; }
inline int GameModel::CityCount() const {
    return static_cast<int>( _board->GetTilesOfType( board::TileType::CITY ).size() );
}

bool GameModel::IsTilePlaceable( const Player* player ) const {
    return _board->GetPlaceableTilesOfType( player, board::TileType::EMPTY ).size() > 0;
}

bool GameModel::IsCityPlaceable( const Player* player ) const {
    return _board->GetValidCityTiles( player ).size() > 0;
}

bool GameModel::IsUrbanizedAreaPlaceable( const Player* player ) const {
    return _board->GetValidUrbanizedAreaTiles( player ).size() > 0;
}

bool GameModel::IsAvailableLonelyTile( const Player* player ) const {
    return _board->GetValidLonelyCityTiles( player ).size() > 0;
}

bool GameModel::AreGlobalParametersFulfilled() const {
    return Temperature() == MAX_TEMPTERATURE && OceanCount() == MAX_OCEAN_COUNT && Oxygen() == MAX_OXYGEN_LEVEL;
}

inline bool GameModel::CanUsePowerPlantSP() const { return _state->CanUsePowerPlantSP(); }
inline bool GameModel::CanUseAsteroidSP() const { return _state->CanUseAsteroidSP(); }
inline bool GameModel::CanUseAquiferSP() const { return _state->CanUseAquiferSP(); }
inline bool GameModel::CanUseGreenerySP() const { return _state->CanUseGreenerySP(); }
inline bool GameModel::CanUseCitySP() const { return _state->CanUseCitySP(); }
inline bool GameModel::CanConvertPlantsToGreenery() const { return _state->CanConvertPlantsToGreenery(); }
inline bool GameModel::CanConvertHeatToTemperature() const { return _state->CanConvertHeatToTemperature(); }

inline bool GameModel::InIdleState() const { return _state->InIdleState(); }
inline bool GameModel::CanPlayCards() const { return InIdleState(); }
inline bool GameModel::CanUseActions() const { return InIdleState(); }

inline void GameModel::SellCardSP( decks::Card* card ) { _state->SellCardSP( card ); }
inline void GameModel::UsePowerPlantSP() { _state->UsePowerPlantSP(); }
inline void GameModel::UseAsteroidSP() { _state->UseAsteroidSP(); }
inline void GameModel::UseAquiferSP() { _state->UseAquiferSP(); }
inline void GameModel::UseGreenerySP() { _state->UseGreenerySP(); }
inline void GameModel::UseCitySP() { _state->UseCitySP(); }

inline void GameModel::PlayCard( decks::Card* card ) { _state->PlayCard( card ); }
inline void GameModel::UseAction( decks::Card* card ) { _state->UseAction( card ); }
inline void GameModel::ConvertPlantsToGreenery() { _state->ConvertPlantsToGreenery(); }
inline void GameModel::ConvertHeatToTemperature() { _state->ConvertHeatToTemperature(); }

inline void GameModel::TilePlacementConfirmed( int q, int r ) { _state->TilePlacementConfirmed( q, r ); }
inline void GameModel::PaymentConfirmed( int credit, int resource ) { _state->PaymentConfirmed( credit, resource ); }

inline void GameModel::EndTurn() { _state->EndTurn(); }

inline void GameModel::SetOnDrawCard( Callback<> callback ) { _on_draw_card.SetCallback( callback ); }
inline void GameModel::SetOnRaiseTR( Callback<int> callback ) { _on_raise_tr.SetCallback( callback ); }
inline void GameModel::SetOnRaiseTemperature( Callback<> callback ) { _on_raise_temperature.SetCallback( callback ); }
inline void GameModel::SetOnRaiseOxygen( Callback<> callback ) { _on_raise_oxygen.SetCallback( callback ); }
inline void GameModel::SetOnResourceAmountChanged( Callback<Resource, int> callback ) { _on_resource_amount_changed.SetCallback( callback ); }
inline void GameModel::SetOnResourceProductionAmountChanged( Callback<Resource, int> callback ) { _on_resource_production_amount_changed.SetCallback( callback ); }
inline void GameModel::SetOnConfirmPayment( Callback<int, Resource, int> callback ) { _on_confirm_payment.SetCallback( callback ); }
inline void GameModel::SetOnConfirmPlacement( Callback<board::TileType, std::vector<pii>> callback ) { _on_confirm_placement.SetCallback( callback ); }

void GameModel::SubscribeCallbacksOnPlayer( Player* player ) {
    player->SetOnDrawCardCallback( std::bind_front( &GameModel::Player_OnDrawCard, this ) );

    player->SetOnRaiseTRCallback( std::bind_front( &GameModel::Player_OnRaiseTR, this ) );

    player->SetOnRaiseTemperatureCallback( std::bind_front( &GameModel::Player_OnRaiseTemperature, this ) );
    player->SetOnPlaceOceanCallback( std::bind_front( &GameModel::Player_OnPlaceOcean, this ) );
    player->SetOnPlaceOceanOnNonOceanCallback( std::bind_front( &GameModel::Player_OnPlaceOceanOnNonOcean, this ) );
    player->SetOnRaiseOxygenCallback( std::bind_front( &GameModel::Player_OnRaiseOxygen, this ) );

    player->SetOnPlaceGreeneryCallback( std::bind_front( &GameModel::Player_OnPlaceGreenery, this ) );
    player->SetOnPlaceGreeneryOnOceanCallback( std::bind_front( &GameModel::Player_OnPlaceGreeneryOnOcean, this ) );
    player->SetOnPlaceCityCallback( std::bind_front( &GameModel::Player_OnPlaceCity, this ) );
    player->SetOnPlaceNoctisCityCallback( std::bind_front( &GameModel::Player_OnPlaceNoctisCity, this ) );
    player->SetOnPlaceLonelyCityCallback( std::bind_front( &GameModel::Player_OnPlaceLonelyCity, this ) );
    player->SetOnPlaceUrbanizedAreaCallback( std::bind_front( &GameModel::Player_OnPlaceUrbanizedArea, this ) );

    player->SetOnResourceAmountChangedCallback( std::bind_front( &GameModel::Player_OnResourceAmountChanged, this ) );
    player->SetOnResourceProductionAmountChangedCallback( std::bind_front( &GameModel::Player_OnResourceProductionAmountChanged, this ) );
    player->SetOnDestroyResourceCallback( std::bind_front( &GameModel::Player_OnDestroyResource, this ) );
    player->SetOnDestroyResourceProductionCallback( std::bind_front( &GameModel::Player_OnDestroyResourceProduction, this ) );

    player->SetOnConfirmSteelPaymentCallback( std::bind_front( &GameModel::Player_OnConfirmSteelPayment, this ) );
    player->SetOnConfirmSteelPaymentCallback( std::bind_front( &GameModel::Player_OnConfirmTitaniumPayment, this ) );
}

inline void GameModel::Player_OnDrawCard( Player* player ) { _state->Player_OnDrawCard( player ); }
inline void GameModel::Player_OnRaiseTR( Player* player, int amount ) { _state->Player_OnRaiseTR( player, amount );}
inline void GameModel::Player_OnRaiseTemperature( Player* player ) { _state->Player_OnRaiseTemperature( player );}
inline void GameModel::Player_OnPlaceOcean( Player* player ) { _state->Player_OnPlaceOcean( player );}
inline void GameModel::Player_OnPlaceOceanOnNonOcean( Player* player ) { _state->Player_OnPlaceOceanOnNonOcean( player );}
inline void GameModel::Player_OnRaiseOxygen( Player* player ) { _state->Player_OnRaiseOxygen( player );}
inline void GameModel::Player_OnPlaceGreenery( Player* player ) { _state->Player_OnPlaceGreenery( player );}
inline void GameModel::Player_OnPlaceGreeneryOnOcean( Player* player ) { _state->Player_OnPlaceGreeneryOnOcean( player );}
inline void GameModel::Player_OnPlaceCity( Player* player ) { _state->Player_OnPlaceCity( player );}
inline void GameModel::Player_OnPlaceNoctisCity( Player* player ) { _state->Player_OnPlaceNoctisCity( player );}
inline void GameModel::Player_OnPlaceLonelyCity( Player* player ) { _state->Player_OnPlaceLonelyCity( player );}
inline void GameModel::Player_OnPlaceUrbanizedArea( Player* player ) { _state->Player_OnPlaceUrbanizedArea( player );}
inline void GameModel::Player_OnResourceAmountChanged( Player* player, Resource resource, int amount )
    { _state->Player_OnResourceAmountChanged( player, resource, amount );}
inline void GameModel::Player_OnResourceProductionAmountChanged( Player* player, Resource resource, int amount )
    { _state->Player_OnResourceProductionAmountChanged( player, resource, amount );}
inline void GameModel::Player_OnDestroyResource( Player* player, Resource resource, int amount )
    { _state->Player_OnDestroyResource( player, resource, amount );}
inline void GameModel::Player_OnDestroyResourceProduction( Player* player, Resource resource, int amount )
    { _state->Player_OnDestroyResourceProduction( player, resource, amount );}
inline void GameModel::Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment )
    { _state->Player_OnConfirmSteelPayment( player, cost, after_payment ); }
inline void GameModel::Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment )
    { _state->Player_OnConfirmTitaniumPayment( player, cost, after_payment); }

#pragma region Requests

GameModel::PlacementRequest::PlacementRequest( board::TileType type, std::vector<pii> valid_positions )
    : type(type), valid_positions( std::move( valid_positions ) ) {}

void GameModel::PlacementRequest::Perform( GameModelState* state ) {
    state->PerformRequest( this );
}

GameModel::PaymentRequest::PaymentRequest( int cost, Resource resource, int resource_value )
    : cost( cost ), resource( resource ), resource_value( resource_value ) {}

void GameModel::PaymentRequest::Perform( GameModelState* state ) {
    state->PerformRequest( this );
}
}

#pragma endregion Request
