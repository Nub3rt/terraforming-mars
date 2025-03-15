#include "game_model.h"

#include <stdexcept>

#include "boards/board.h"
#include "decks/card.h"
#include "constants.h"
#include "decks/active_card_with_action.h"
#include "decks/deck.h"
#include "event.h"
#include "game_model_state.h"
#include "player.h"

namespace model
{
template<typename... Args>
using Callback = std::function<void( Args... )>;

model::GameModel::GameModel( int seed ) : _random( seed ) {}

GameModel::~GameModel() {
    if ( !_initialized )
        return;

    delete _board;
    delete _deck;
    delete _local_player;
    delete _state;

    while ( !_queued_request.empty() ) {
        Request* request = _queued_request.front();
        _queued_request.pop();
        delete request;
    }
}

void GameModel::Initialize( boards::Board* board, decks::Deck* deck ) {
    if ( _initialized )
        throw std::logic_error( "GameModel::Initialize: model was already initialized!" );

    _initialized = true;

    _generation = 1;
    _temperature = STARTING_TEMPERATURE;
    _ocean_count = STARTING_OCEAN_COUNT;
    _oxygen_level = STARTING_OXYGEN_LEVEL;

    _state = CreateIdleState();

    _board = board;
    _deck = deck;

    _local_player = CreateLocalPlayer();
}

void GameModel::Start() {
    if ( !_initialized )
        throw std::logic_error( "GameModel::Start: model was not initialized!" );

    if ( _started )
        throw std::logic_error( "GameModel::Start: model was already started!" );

    _started = true;

    // TODO
}

inline int GameModel::get_generation() const { return _generation; }
inline const boards::Board* GameModel::get_board() const { return _board; }
inline const Player* GameModel::get_local_player() const { return _local_player; }

inline int GameModel::Temperature() const { return _temperature; }
inline int GameModel::OceanCount() const { return _ocean_count; }
inline int GameModel::Oxygen() const { return _oxygen_level; }

inline int GameModel::CityCount() const {
    return static_cast<int>( _board->GetTilesOfType( boards::TileType::CITY ).size() );
}

bool GameModel::IsTilePlaceable( const Player* player ) const {
    return _board->GetPlaceableTilesOfType( player, boards::TileType::EMPTY ).size() > 0;
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
    return Temperature() == MAX_TEMPERATURE && OceanCount() == MAX_OCEAN_COUNT && Oxygen() == MAX_OXYGEN_LEVEL;
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
inline void GameModel::ConvertPlantsToGreenery() { _state->ConvertPlantsToGreenery(); }
inline void GameModel::ConvertHeatToTemperature() { _state->ConvertHeatToTemperature(); }

inline void GameModel::PlayCard( decks::Card* card ) { _state->PlayCard( card ); }
inline void GameModel::UseAction( decks::ActiveCardWithAction* card ) { _state->UseAction( card ); }

inline void GameModel::ToggleToBuyCard( int index ) { _state->ToggleToBuyCard( index ); }
inline int GameModel::GetTotalCost() const { return _state->GetTotalCost(); }
inline void GameModel::ConfirmPurchases() { _state->ConfirmPurchases(); }

inline void GameModel::TilePlacementConfirmed( int q, int r ) { _state->TilePlacementConfirmed( q, r ); }
inline void GameModel::PaymentConfirmed( int credit, int resource ) { _state->PaymentConfirmed( credit, resource ); }

inline void GameModel::EndTurn() { _state->EndTurn(); }

void GameModel::EndGame() {
    _game_ended = true;
    _on_game_end.Invoke();
    ChangeState( CreateGameOverState() );
}

inline void GameModel::SetOnDrawCard( Callback<decks::Card*> callback ) { _on_draw_card.SetCallback( callback ); }
inline void GameModel::SetOnPlayCard( Callback<decks::Card*> callback ) { _on_play_card.SetCallback( callback ); }
inline void GameModel::SetOnRaiseTR( Callback<int> callback ) { _on_raise_tr.SetCallback( callback ); }
inline void GameModel::SetOnRaiseTemperature( Callback<> callback ) { _on_raise_temperature.SetCallback( callback ); }
inline void GameModel::SetOnRaiseOxygen( Callback<> callback ) { _on_raise_oxygen.SetCallback( callback ); }
inline void GameModel::SetOnPlaceTile( Callback<pii> callback ) { _on_place_tile.SetCallback( callback ); }
inline void GameModel::SetOnResourceAmountChanged( Callback<Resource, int> callback ) { _on_resource_amount_changed.SetCallback( callback ); }
inline void GameModel::SetOnResourceProductionAmountChanged( Callback<Resource, int> callback ) { _on_resource_production_amount_changed.SetCallback( callback ); }
inline void GameModel::SetOnResearchConfirmed( Callback<std::array<bool, RESEARCH_CARD_NUM>> callback ) { _on_research_confirmed.SetCallback( callback ); }
inline void GameModel::SetOnConfirmResearch( Callback<std::array<decks::Card*, RESEARCH_CARD_NUM>> callback ) { _on_confirm_research.SetCallback( callback ); }
inline void GameModel::SetOnConfirmPayment( Callback<int, Resource, int> callback ) { _on_confirm_payment.SetCallback( callback ); }
inline void GameModel::SetOnConfirmPlacement( Callback<boards::TileType, std::vector<pii>> callback ) { _on_confirm_placement.SetCallback( callback ); }
inline void GameModel::SetOnConfirmDestroyResource( Callback<Resource, int> callback ) { _on_confirm_destroy_resource.SetCallback( callback ); }
inline void GameModel::SetOnConfirmDestroyResourceProduction( Callback<Resource, int> callback ) { _on_confirm_destroy_resource_production.SetCallback( callback ); }
inline void GameModel::SetOnGameEnd( Callback<> callback ) { _on_game_end.SetCallback( callback ); }

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

IdleState* GameModel::CreateIdleState() { return new IdleState( this ); }
ResearchState* GameModel::CreateResearchState() { return new ResearchState( this ); }
PlacementConfirmationState* GameModel::CreatePlacementConfirmationState( PlacementRequest* request ) { return new PlacementConfirmationState( this, request ); }
PaymentConfirmationState* GameModel::CreatePaymentConfirmationState( PaymentRequest* request ) { return new PaymentConfirmationState( this, request ); }
PostLastGenerationState* GameModel::CreatePostLastGenerationState() { return new PostLastGenerationState( this ); }
PostLastGenerationPlacementConfirmationState* GameModel::CreatePostLastGenerationPlacementConfirmationState( PostLastGenerationGreeneryPlacementRequest* request ) { return new PostLastGenerationPlacementConfirmationState( this, request ); }
GameOverState* GameModel::CreateGameOverState() { return new GameOverState( this ); }

void GameModel::ChangeState( GameModelState* state ) {
    if ( _state != nullptr )
        delete _state;

    _state = state;
}

inline void GameModel::Player_OnDrawCard( Player* player ) { _state->Player_OnDrawCard( player ); }
inline void GameModel::Player_OnRaiseTR( Player* player, int amount ) { _state->Player_OnRaiseTR( player, amount ); }
inline void GameModel::Player_OnRaiseTemperature( Player* player ) { _state->Player_OnRaiseTemperature( player ); }
inline void GameModel::Player_OnPlaceOcean( Player* player ) { _state->Player_OnPlaceOcean( player ); }
inline void GameModel::Player_OnPlaceOceanOnNonOcean( Player* player ) { _state->Player_OnPlaceOceanOnNonOcean( player ); }
inline void GameModel::Player_OnRaiseOxygen( Player* player ) { _state->Player_OnRaiseOxygen( player ); }
inline void GameModel::Player_OnPlaceGreenery( Player* player ) { _state->Player_OnPlaceGreenery( player ); }
inline void GameModel::Player_OnPlaceGreeneryOnOcean( Player* player ) { _state->Player_OnPlaceGreeneryOnOcean( player ); }
inline void GameModel::Player_OnPlaceCity( Player* player ) { _state->Player_OnPlaceCity( player ); }
inline void GameModel::Player_OnPlaceNoctisCity( Player* player ) { _state->Player_OnPlaceNoctisCity( player ); }
inline void GameModel::Player_OnPlaceLonelyCity( Player* player ) { _state->Player_OnPlaceLonelyCity( player ); }
inline void GameModel::Player_OnPlaceUrbanizedArea( Player* player ) { _state->Player_OnPlaceUrbanizedArea( player ); }
inline void GameModel::Player_OnResourceAmountChanged( Player* player, Resource resource, int amount )
    { _state->Player_OnResourceAmountChanged( player, resource, amount ); }
inline void GameModel::Player_OnResourceProductionAmountChanged( Player* player, Resource resource, int amount )
    { _state->Player_OnResourceProductionAmountChanged( player, resource, amount ); }
inline void GameModel::Player_OnDestroyResource( Player* player, Resource resource, int amount )
    { _state->Player_OnDestroyResource( player, resource, amount ); }
inline void GameModel::Player_OnDestroyResourceProduction( Player* player, Resource resource, int amount )
    { _state->Player_OnDestroyResourceProduction( player, resource, amount ); }
inline void GameModel::Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment )
    { _state->Player_OnConfirmSteelPayment( player, cost, after_payment ); }
inline void GameModel::Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment )
    { _state->Player_OnConfirmTitaniumPayment( player, cost, after_payment); }

#pragma region Requests

GameModel::PlacementRequest::PlacementRequest( boards::TileType type, std::vector<pii> valid_positions )
    : type( type ), valid_positions( std::move( valid_positions ) ) {}

void GameModel::PlacementRequest::Perform( GameModelState* state ) {
    state->PerformRequest( this );
}

GameModel::PaymentRequest::PaymentRequest( int cost, Resource resource, int resource_value, std::function<void()> after_payment )
    : cost( cost ), resource( resource ), resource_value( resource_value ), after_payment( std::move( after_payment ) ) {}

void GameModel::PaymentRequest::Perform( GameModelState* state ) {
    state->PerformRequest( this );
}

GameModel::PostLastGenerationGreeneryPlacementRequest::PostLastGenerationGreeneryPlacementRequest( std::vector<pii> valid_positions )
    : valid_positions( std::move( valid_positions ) ) {}

void GameModel::PostLastGenerationGreeneryPlacementRequest::Perform( GameModelState* state ) {
    state->PerformRequest( this );
}

#pragma endregion Requests
}
