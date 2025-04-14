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

    if ( _next_state != nullptr )
        delete _next_state;

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
    _board->SetOnTilePlacedCallback( [ this ]( int q, int r, const boards::Tile& tile ) {
        _on_place_tile.Invoke( std::pair( q, r ) );
    } );

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

void GameModel::Update() {
    if ( _next_state != nullptr ) {
        delete _state;
        _state = _next_state;
        _next_state = nullptr;
    }
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

bool GameModel::CanUsePowerPlantSP() const { return _state->CanUsePowerPlantSP(); }
bool GameModel::CanUseAsteroidSP() const { return _state->CanUseAsteroidSP(); }
bool GameModel::CanUseAquiferSP() const { return _state->CanUseAquiferSP(); }
bool GameModel::CanUseGreenerySP() const { return _state->CanUseGreenerySP(); }
bool GameModel::CanUseCitySP() const { return _state->CanUseCitySP(); }
bool GameModel::CanConvertPlantsToGreenery() const { return _state->CanConvertPlantsToGreenery(); }
bool GameModel::CanConvertHeatToTemperature() const { return _state->CanConvertHeatToTemperature(); }

bool GameModel::InIdleState() const { return _state->InIdleState(); }
bool GameModel::CanPlayCards() const { return InIdleState(); }
bool GameModel::CanUseActions() const { return InIdleState(); }

void GameModel::SellCardSP( const decks::Card* card ) { _state->SellCardSP( card ); }
void GameModel::UsePowerPlantSP() { _state->UsePowerPlantSP(); }
void GameModel::UseAsteroidSP() { _state->UseAsteroidSP(); }
void GameModel::UseAquiferSP() { _state->UseAquiferSP(); }
void GameModel::UseGreenerySP() { _state->UseGreenerySP(); }
void GameModel::UseCitySP() { _state->UseCitySP(); }
void GameModel::ConvertPlantsToGreenery() { _state->ConvertPlantsToGreenery(); }
void GameModel::ConvertHeatToTemperature() { _state->ConvertHeatToTemperature(); }

void GameModel::PlayCard( const decks::Card* card ) { _state->PlayCard( card ); }
void GameModel::UseAction( const decks::ActiveCardWithAction* card ) { _state->UseAction( card ); }

void GameModel::ToggleToBuyCard( int index ) { _state->ToggleToBuyCard( index ); }
int GameModel::GetTotalCost() const { return _state->GetTotalCost(); }
void GameModel::ConfirmPurchases() { _state->ConfirmPurchases(); }

void GameModel::TilePlacementConfirmed( int q, int r ) { _state->TilePlacementConfirmed( q, r ); }
void GameModel::PaymentConfirmed( int credit, int resource ) { _state->PaymentConfirmed( credit, resource ); }

void GameModel::EndTurn() { _state->EndTurn(); }

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
    player->SetOnConfirmTitaniumPaymentCallback( std::bind_front( &GameModel::Player_OnConfirmTitaniumPayment, this ) );
}

IdleState* GameModel::CreateIdleState() { return new IdleState( this ); }
ResearchState* GameModel::CreateResearchState() { return new ResearchState( this ); }
PlacementConfirmationState* GameModel::CreatePlacementConfirmationState( PlacementRequest* request ) { return new PlacementConfirmationState( this, request ); }
PaymentConfirmationState* GameModel::CreatePaymentConfirmationState( PaymentRequest* request ) { return new PaymentConfirmationState( this, request ); }
PostLastGenerationState* GameModel::CreatePostLastGenerationState() { return new PostLastGenerationState( this ); }
PostLastGenerationPlacementConfirmationState* GameModel::CreatePostLastGenerationPlacementConfirmationState( PostLastGenerationGreeneryPlacementRequest* request ) { return new PostLastGenerationPlacementConfirmationState( this, request ); }
GameOverState* GameModel::CreateGameOverState() { return new GameOverState( this ); }

void GameModel::RequestStateChange( GameModelState* state ) {
    if ( _next_state == nullptr )
        _next_state = state;
    else
        throw std::logic_error( "GameModel::RequestStateChange: another state change was already requested!" );
}

void GameModel::Player_OnDrawCard( Player* player ) { _state->Player_OnDrawCard( player ); }
void GameModel::Player_OnRaiseTR( Player* player, int amount ) { _state->Player_OnRaiseTR( player, amount ); }
void GameModel::Player_OnRaiseTemperature( Player* player ) { _state->Player_OnRaiseTemperature( player ); }
void GameModel::Player_OnPlaceOcean( Player* player ) { _state->Player_OnPlaceOcean( player ); }
void GameModel::Player_OnPlaceOceanOnNonOcean( Player* player ) { _state->Player_OnPlaceOceanOnNonOcean( player ); }
void GameModel::Player_OnRaiseOxygen( Player* player ) { _state->Player_OnRaiseOxygen( player ); }
void GameModel::Player_OnPlaceGreenery( Player* player ) { _state->Player_OnPlaceGreenery( player ); }
void GameModel::Player_OnPlaceGreeneryOnOcean( Player* player ) { _state->Player_OnPlaceGreeneryOnOcean( player ); }
void GameModel::Player_OnPlaceCity( Player* player ) { _state->Player_OnPlaceCity( player ); }
void GameModel::Player_OnPlaceNoctisCity( Player* player ) { _state->Player_OnPlaceNoctisCity( player ); }
void GameModel::Player_OnPlaceLonelyCity( Player* player ) { _state->Player_OnPlaceLonelyCity( player ); }
void GameModel::Player_OnPlaceUrbanizedArea( Player* player ) { _state->Player_OnPlaceUrbanizedArea( player ); }
void GameModel::Player_OnResourceAmountChanged( Player* player, Resource resource, int amount )
    { _state->Player_OnResourceAmountChanged( player, resource, amount ); }
void GameModel::Player_OnResourceProductionAmountChanged( Player* player, Resource resource, int amount )
    { _state->Player_OnResourceProductionAmountChanged( player, resource, amount ); }
void GameModel::Player_OnDestroyResource( Player* player, Resource resource, int amount )
    { _state->Player_OnDestroyResource( player, resource, amount ); }
void GameModel::Player_OnDestroyResourceProduction( Player* player, Resource resource, int amount )
    { _state->Player_OnDestroyResourceProduction( player, resource, amount ); }
void GameModel::Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment )
    { _state->Player_OnConfirmSteelPayment( player, cost, after_payment ); }
void GameModel::Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment )
    { _state->Player_OnConfirmTitaniumPayment( player, cost, after_payment); }

void GameModel::EndGame() {
    _game_ended = true;
    _on_game_end.Invoke();
    RequestStateChange( CreateGameOverState() );
}
#pragma region Requests

GameModel::PlacementRequest::PlacementRequest( boards::TileType type, std::function<std::vector<std::pair<int, int>>()> get_valid_positions )
    : type( type ), get_valid_positions( get_valid_positions ) {}

void GameModel::PlacementRequest::Perform( GameModelState* state ) {
    state->PerformRequest( this );
}

GameModel::PaymentRequest::PaymentRequest( int cost, Resource resource, int resource_value, std::function<void()> after_payment )
    : cost( cost ), resource( resource ), resource_value( resource_value ), after_payment( std::move( after_payment ) ) {}

void GameModel::PaymentRequest::Perform( GameModelState* state ) {
    state->PerformRequest( this );
}

GameModel::PostLastGenerationGreeneryPlacementRequest::PostLastGenerationGreeneryPlacementRequest( std::vector<std::pair<int, int>> valid_positions )
    : valid_positions( std::move( valid_positions ) ) {}

void GameModel::PostLastGenerationGreeneryPlacementRequest::Perform( GameModelState* state ) {
    state->PerformRequest( this );
}

#pragma endregion Requests
}
