#include "game_model_state.hpp"

#include <format>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

#include "constants.hpp"
#include "game_model.hpp"
#include "player.hpp"
#include "resource.hpp"

#include "decks/card.hpp"
#include "decks/deck.hpp"

namespace model
{
#pragma region GameModelState

GameModelState::GameModelState( GameModel* model ) : _model( model ) {}

bool GameModelState::CanUsePowerPlantSP() { return false; }
bool GameModelState::CanUseAsteroidSP() { return false; }
bool GameModelState::CanUseAquiferSP() { return false; }
bool GameModelState::CanUseGreenerySP() { return false; }
bool GameModelState::CanUseCitySP() { return false; }
bool GameModelState::CanConvertPlantsToGreenery() { return false; }
bool GameModelState::CanConvertHeatToTemperature() { return false; }

bool GameModelState::InIdleState() { return false; }

void GameModelState::SellCardSP( const decks::Card* card ) { throw std::logic_error( "GameModelState::SellCardSP: GameModel was in an invalid state!" ); }
void GameModelState::UsePowerPlantSP() { throw std::logic_error( "GameModelState::UsePowerPlantSP: GameModel was in an invalid state!" ); }
void GameModelState::UseAsteroidSP() { throw std::logic_error( "GameModelState::UseAsteroidSP: GameModel was in an invalid state!" ); }
void GameModelState::UseAquiferSP() { throw std::logic_error( "GameModelState::UseAquiferSP: GameModel was in an invalid state!" ); }
void GameModelState::UseGreenerySP() { throw std::logic_error( "GameModelState::UseGreenerySP: GameModel was in an invalid state!" ); }
void GameModelState::UseCitySP() { throw std::logic_error( "GameModelState::UseCitySP: GameModel was in an invalid state!" ); }
void GameModelState::ConvertPlantsToGreenery() { throw std::logic_error( "GameModelState::ConvertPlantsToGreenery: GameModel was in an invalid state!" ); }
void GameModelState::ConvertHeatToTemperature() { throw std::logic_error( "GameModelState::ConvertHeatToTemperature: GameModel was in an invalid state!" ); }

void GameModelState::PlayCard( const decks::Card* card ) { throw std::logic_error( "GameModelState::PlayCard: GameModel was in an invalid state!" ); }

void GameModelState::ToggleToBuyCard( int index ) { throw std::logic_error( "GameModelState::ToggleToBuyCard: GameModel was in an invalid state!" ); }
int GameModelState::GetTotalCost() const { throw std::logic_error( "GameModelState::GetTotalCost: GameModel was in an invalid state!" ); }
void GameModelState::ConfirmPurchases() { throw std::logic_error( "GameModelState::ConfirmPurchases: GameModel was in an invalid state!" ); }

void GameModelState::TilePlacementConfirmed( int q, int r ) { throw std::logic_error( "GameModelState::TilePlacementConfirmed: GameModel was in an invalid state!" ); }

void GameModelState::PaymentConfirmed( int credit, int resource ) { throw std::logic_error( "GameModelState::PaymentConfirmed: GameModel was in an invalid state!" ); }

void GameModelState::EndTurn() { throw std::logic_error( "GameModelState::EndTurn: GameModel was in an invalid state!" ); }

void GameModelState::PerformRequest( GameModel::PlacementRequest* request ) {
    if ( _model->_next_state == nullptr ) {
        if ( request->type == boards::TileType::OCEAN && _model->_ocean_count == MAX_OCEAN_COUNT ) {
            _model->RequestStateChange( _model->CreateIdleState() );
            return;
        }

        PlacementConfirmationState* state = _model->CreatePlacementConfirmationState( request );
        _model->_on_confirm_placement.Invoke( request->type, state->valid_positions );
        _model->RequestStateChange( state );
    } else {
        _model->_queued_request.push( request );
    }
}

void GameModelState::PerformRequest( GameModel::PaymentRequest* request ) {
    if ( _model->_next_state == nullptr ) {
        _model->_on_confirm_payment.Invoke( request->cost, request->resource, request->resource_value );
        _model->RequestStateChange( _model->CreatePaymentConfirmationState( request ) );
    } else {
        _model->_queued_request.push( request );
    }
}

void GameModelState::PerformRequest( GameModel::PostLastGenerationGreeneryPlacementRequest* request ) {
    if ( _model->_next_state == nullptr ) {
        _model->_on_confirm_placement.Invoke( boards::TileType::GREENERY, request->valid_positions );
        _model->RequestStateChange( _model->CreatePostLastGenerationPlacementConfirmationState( request ) );
    } else {
        _model->_queued_request.push( request );
    }
}

void GameModelState::Player_OnDrawCard( Player* player ) {
    decks::Card* card = _model->_deck->DrawCard();

    if ( card == nullptr )
        return;

    player->GainCard( card );
    _model->_on_draw_card.Invoke( card );
}

void GameModelState::Player_OnRaiseTR( Player* player, int amount ) {
    player->GainTR( amount );
    _model->_on_raise_tr.Invoke( amount );
}

void GameModelState::Player_OnRaiseTemperature( Player* player ) {
    if ( _model->Temperature() == MAX_TEMPERATURE )
        return;

    _model->_temperature += 2;
    _model->_on_raise_temperature.Invoke();
    player->RaiseTR( 1 );
}

void GameModelState::Player_OnRaiseOxygen( Player* player ) {
    if ( _model->Oxygen() == MAX_OXYGEN_LEVEL )
        return;

    _model->_oxygen_level += 1;
    _model->_on_raise_oxygen.Invoke();
    player->RaiseTR( 1 );
}

void GameModelState::Player_OnPlaceOcean( Player* player ) {
    DoOnPlacementConfirmation( boards::TileType::OCEAN, [ board = _model->_board, player ]() {
        return board->GetValidOceanTiles( player );
    } );
}

void GameModelState::Player_OnPlaceOceanOnNonOcean( Player* player ) {

    DoOnPlacementConfirmation( boards::TileType::OCEAN, [ board = _model->_board, player ]() {
        return board->GetEmptyTiles( player );
    } );
}

void GameModelState::Player_OnPlaceGreenery( Player* player ) {

    DoOnPlacementConfirmation( boards::TileType::GREENERY, [ board = _model->_board, player ]() {
        return board->GetValidGreeneryTiles( player );
    } );
}

void GameModelState::Player_OnPlaceGreeneryOnOcean( Player* player ) {

    DoOnPlacementConfirmation( boards::TileType::GREENERY, [ board = _model->_board, player ]() {
        return board->GetValidOceanTiles( player );
    } );
}

void GameModelState::Player_OnPlaceCity( Player* player ) {

    DoOnPlacementConfirmation( boards::TileType::CITY, [ board = _model->_board, player ]() {
        return board->GetValidCityTiles( player );
    } );
}

void GameModelState::Player_OnPlaceLonelyCity( Player* player ) {

    DoOnPlacementConfirmation( boards::TileType::CITY, [ board = _model->_board, player ]() {
        return board->GetValidLonelyCityTiles( player );
    } );
}

void GameModelState::Player_OnPlaceUrbanizedArea( Player* player ) {

    DoOnPlacementConfirmation( boards::TileType::CITY, [ board = _model->_board, player ]() {
        return board->GetValidUrbanizedAreaTiles( player );
    } );
}

void GameModelState::Player_OnPlaceNoctisCity( Player* player ) {
    std::vector<std::pair<int, int>> valid_positions = _model->_board->GetValidNoctisCityTiles( player );

    if ( valid_positions.size() == 0 )
        throw std::logic_error( "GameModelState::Player_OnPlaceNoctisCity: no positions to place tile!" );

    if ( valid_positions.size() == 1 ) {
        auto& [q, r] = valid_positions[ 0 ];
        _model->_board->PlaceTile( q, r, player, boards::TileType::CITY );

        int oceans = (int)_model->_board->GetNeighbouringTilesOfType( q, r, boards::TileType::OCEAN ).size();
        if ( oceans != 0 )
            _model->_local_player->GainResource( Resource::CREDIT, oceans * 2 );

        return;
    }

    DoOnPlacementConfirmation( boards::TileType::CITY, [ valid_positions ]() { return valid_positions; } );
}

void GameModelState::Player_OnResourceAmountChanged( Player* player, Resource resource, int amount ) {
    _model->_on_resource_amount_changed.Invoke( resource, amount );
}

void GameModelState::Player_OnResourceProductionAmountChanged( Player* player, Resource resource, int amount ) {
    _model->_on_resource_production_amount_changed.Invoke( resource, amount );
}

void GameModelState::Player_OnDestroyResource( Player* player, Resource resource, int amount ) {
    // TODO
}

void GameModelState::Player_OnDestroyResourceProduction( Player* player, Resource resource, int amount ) {
    // TODO
}

void GameModelState::Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment ) {
    throw std::logic_error( "GameModelState::Player_OnConfirmSteelPayment: GameModel was in an invalid state!" );
}

void GameModelState::Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment ) {
    throw std::logic_error( "GameModelState::Player_OnConfirmTitaniumPayment: GameModel was in an invalid state!" );
}

void GameModelState::DoOnPlacementConfirmation( boards::TileType type, std::function<std::vector<std::pair<int, int>>()> get_valid_positions ) {
    throw std::logic_error( "GameModelState::DoOnPlacementConfirmation: GameModel was in an invalid state!" );
}

#pragma endregion GameModelState

#pragma region ResearchState

ResearchState::ResearchState( GameModel* model ) : GameModelState( model ), _cards(), _to_buy() {
    std::array<const decks::Card*, RESEARCH_CARD_NUM> event_arg;

    for ( int i = 0; i < RESEARCH_CARD_NUM; ++i ) {
        _cards[ i ] = _model->_deck->DrawCard();
        _to_buy[ i ] = true;
        event_arg[ i ] = _cards[ i ];
    }

    _model->_on_confirm_research.Invoke( std::move( event_arg ) );
}

ResearchState::~ResearchState() {
    if ( !_completed ) {
        for ( decks::Card* card : _cards )
            delete card;
    }
}

void ResearchState::ToggleToBuyCard( int index ) {
    _to_buy[ index ] = !_to_buy[ index ];
}

int ResearchState::GetTotalCost() const {
    int sum = 0;

    for ( const bool& to_buy : _to_buy )
        if ( to_buy )
            sum += RESEARCH_CARD_COST;

    return sum;
}

void ResearchState::ConfirmPurchases() {
    int total_cost = GetTotalCost();
    if ( total_cost != 0 )
        _model->_local_player->LoseResource( Resource::CREDIT, total_cost );

    for ( int i = 0; i < RESEARCH_CARD_NUM; ++i )
        if ( _to_buy[ i ] )
            _model->_local_player->GainCard( _cards[ i ] );
        else
            _model->_deck->DiscardCard( _cards[ i ] );

    _completed = true;

    _model->_on_research_confirmed.Invoke( std::move( _to_buy ) );

    _model->RequestStateChange( _model->CreateIdleState() );
}

#pragma endregion ResearchState

#pragma region IdleState

IdleState::IdleState( GameModel* model ) : GameModelState( model ) {}
IdleState::~IdleState() {}

bool IdleState::CanUsePowerPlantSP() { return _model->_local_player->GetResource( Resource::CREDIT ) >= POWER_PLANT_SP_COST; }
bool IdleState::CanUseAsteroidSP() { return _model->_local_player->GetResource( Resource::CREDIT ) >= ASTEROID_SP_COST &&
                                            _model->Temperature() < MAX_TEMPERATURE; }
bool IdleState::CanUseAquiferSP() { return _model->_local_player->GetResource( Resource::CREDIT ) >= POWER_PLANT_SP_COST &&
                                           _model->OceanCount() < MAX_OCEAN_COUNT; }
bool IdleState::CanUseGreenerySP() { return _model->_local_player->GetResource( Resource::CREDIT ) >= GREENERY_SP_COST &&
                                            _model->_board->GetValidGreeneryTiles( _model->_local_player ).size() > 0; }
bool IdleState::CanUseCitySP() { return _model->_local_player->GetResource( Resource::CREDIT ) >= POWER_PLANT_SP_COST &&
                                        _model->_board->GetValidCityTiles( _model->_local_player ).size() > 0 ; }
bool IdleState::CanConvertPlantsToGreenery() { return _model->_local_player->GetResource( Resource::PLANTS ) >= _model->_local_player->get_greenery_cost() &&
                                                      _model->_board->GetValidGreeneryTiles( _model->_local_player ).size() > 0; }
bool IdleState::CanConvertHeatToTemperature() { return _model->_local_player->GetResource( Resource::HEAT ) >= _model->_local_player->get_temperature_cost() &&
                                                       _model->Temperature() < MAX_TEMPERATURE; }

bool IdleState::InIdleState() { return true; }

void IdleState::SellCardSP( const decks::Card* card ) {
    _model->_local_player->SellCard( card );
    _model->_deck->DiscardCard( const_cast<decks::Card*>( card ) );
}

void IdleState::UsePowerPlantSP() {
    if ( !CanUsePowerPlantSP() )
        throw std::logic_error( "IdleState::UsePowerPlantSP: standard project cannot be used!" );

    _model->_local_player->LoseResource( Resource::CREDIT, POWER_PLANT_SP_COST );
    _model->_local_player->GainResourceProduction( Resource::ENERGY, 1 );
}

void IdleState::UseAsteroidSP() {
    if ( !CanUseAsteroidSP() )
        throw std::logic_error( "IdleState::UseAsteroidSP: standard project cannot be used!" );

    _model->_local_player->LoseResource( Resource::CREDIT, ASTEROID_SP_COST );
    _model->_local_player->RaiseTemperature();
}
void IdleState::UseAquiferSP() {
    if ( !CanUseAquiferSP() )
        throw std::logic_error( "IdleState::UseAquiferSP: standard project cannot be used!" );

    _model->_local_player->LoseResource( Resource::CREDIT, AQUIFER_SP_COST );
    _model->_local_player->PlaceOcean();
}
void IdleState::UseGreenerySP() {
    if ( !CanUseGreenerySP() )
        throw std::logic_error( "IdleState::UseGreenerySP: standard project cannot be used!" );

    _model->_local_player->LoseResource( Resource::CREDIT, GREENERY_SP_COST );
    _model->_local_player->PlaceGreenery();
}
void IdleState::UseCitySP() {
    if ( !CanUseCitySP() )
        throw std::logic_error( "IdleState::UseCitySP: standard project cannot be used!" );

    _model->_local_player->LoseResource( Resource::CREDIT, CITY_SP_COST );
    _model->_local_player->GainResourceProduction( Resource::CREDIT, 1 );
    _model->_local_player->PlaceCity();
}

void IdleState::ConvertPlantsToGreenery() {
    if ( !CanConvertPlantsToGreenery() )
        throw std::logic_error( "IdleState::ConvertPlantsToGreenery: cannot convert resources at this time!" );

    _model->_local_player->LoseResource( Resource::PLANTS, _model->_local_player->get_greenery_cost() );
    _model->_local_player->PlaceGreenery();
}
void IdleState::ConvertHeatToTemperature() {
    if ( !CanConvertHeatToTemperature() )
        throw std::logic_error( "IdleState::ConvertHeatToTemperature: cannot convert resources at this time!" );

    _model->_local_player->LoseResource( Resource::HEAT, _model->_local_player->get_temperature_cost() );
    _model->_local_player->RaiseTemperature();
}

void IdleState::EndTurn() {
    // TODO
}

void IdleState::PlayCard( const decks::Card* card ) {
    _model->_on_play_card.Invoke( card );
    _model->_local_player->PlayCard( card );
}


void IdleState::Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment ) {
    GameModel::PaymentRequest* request = new GameModel::PaymentRequest( cost, Resource::STEEL, player->get_steel_value(), std::move( after_payment ) );
    PerformRequest( request );
}

void IdleState::Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment ) {
    GameModel::PaymentRequest* request = new GameModel::PaymentRequest( cost, Resource::TITANIUM, player->get_titanium_value(), std::move( after_payment ) );
    PerformRequest( request );
}

void IdleState::DoOnPlacementConfirmation( boards::TileType type, std::function<std::vector<std::pair<int, int>>()> get_valid_positions ) {
    GameModel::PlacementRequest* request = new GameModel::PlacementRequest( type, get_valid_positions );
    PerformRequest( request );
}

#pragma endregion IdleState

#pragma region PlacementConfirmationState

PlacementConfirmationState::PlacementConfirmationState( GameModel* model, GameModel::PlacementRequest* request )
    : GameModelState( model ), _request( request ) {
    if ( request == nullptr )
        throw std::logic_error( "PlacementConfirmationState::ctor: request was null!" );

    valid_positions = _request->get_valid_positions();
}

PlacementConfirmationState::~PlacementConfirmationState() {
    delete _request;
}

void PlacementConfirmationState::TilePlacementConfirmed( int q, int r ) {
    if ( std::find( valid_positions.cbegin(), valid_positions.cend(), std::pair<int, int>( q, r ) ) == valid_positions.cend() )
        throw std::logic_error( std::format( "PlacementConfirmationState::TilePlacementConfirmed: indices q: {}, r: {} are not valid positions!", q, r ) );

    _model->_board->PlaceTile( q, r, _model->_local_player, _request->type );

    int oceans = (int)_model->_board->GetNeighbouringTilesOfType( q, r, boards::TileType::OCEAN ).size();
    if ( oceans != 0 )
        _model->_local_player->GainResource( Resource::CREDIT, oceans * 2 );

    if ( _request->type == boards::TileType::OCEAN ) {
        _model->_ocean_count += 1;
        _model->_local_player->RaiseTR( 1 );
    }

    if ( _request->type == boards::TileType::GREENERY )
        _model->_local_player->RaiseOxygen();

    if ( _model->_queued_request.empty() ) {
        _model->RequestStateChange( _model->CreateIdleState() );
        return;
    }

    GameModel::Request* request = _model->_queued_request.front();
    _model->_queued_request.pop();
    request->Perform( this );
}

void PlacementConfirmationState::Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment ) {
    GameModel::PaymentRequest* request = new GameModel::PaymentRequest( cost, Resource::STEEL, player->get_steel_value(), std::move( after_payment ) );
    _model->_queued_request.push( request );
}

void PlacementConfirmationState::Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment ) {
    GameModel::PaymentRequest* request = new GameModel::PaymentRequest( cost, Resource::TITANIUM, player->get_titanium_value(), std::move( after_payment ) );
    _model->_queued_request.push( request );
}

void PlacementConfirmationState::DoOnPlacementConfirmation( boards::TileType type, std::function<std::vector<std::pair<int, int>>()> get_valid_positions ) {
    GameModel::PlacementRequest* request = new GameModel::PlacementRequest( type, get_valid_positions );
    _model->_queued_request.push( request );
}

#pragma endregion PlacementConfirmationState

#pragma region PaymentConfirmationState

PaymentConfirmationState::PaymentConfirmationState( GameModel* model, GameModel::PaymentRequest* request )
    : GameModelState( model ), _request( request ) {
    if ( request == nullptr )
        throw std::logic_error( "PlacementConfirmationState::ctor: request was null!" );
}

PaymentConfirmationState::~PaymentConfirmationState() {
    delete _request;
}

void PaymentConfirmationState::PaymentConfirmed( int credit, int resource ) {
    if ( credit + resource * _request->resource_value < _request->cost )
        throw std::logic_error( std::format( "PaymentConfirmationState::PaymentConfirmed: values received do not satisfy the cost!\n\
\tcredit + resource * resource_value < cost: {} + {} * {} < {}", credit, resource, _request->resource_value, _request->cost ) );

    if ( credit != 0 )
        _model->_local_player->LoseResource( Resource::CREDIT, credit );
    if ( resource != 0 )
        _model->_local_player->LoseResource( _request->resource, resource );
    _request->after_payment();

    if ( _model->_queued_request.empty() ) {
        _model->RequestStateChange( _model->CreateIdleState() );
        return;
    }

    GameModel::Request* request = _model->_queued_request.front();
    _model->_queued_request.pop();
    request->Perform( this );
}

void PaymentConfirmationState::Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment ) {
    GameModel::PaymentRequest* request = new GameModel::PaymentRequest( cost, Resource::STEEL, player->get_steel_value(), std::move( after_payment ) );
    _model->_queued_request.push( request );
}

void PaymentConfirmationState::Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment ) {
    GameModel::PaymentRequest* request = new GameModel::PaymentRequest( cost, Resource::TITANIUM, player->get_titanium_value(), std::move( after_payment ) );
    _model->_queued_request.push( request );
}

void PaymentConfirmationState::DoOnPlacementConfirmation( boards::TileType type, std::function<std::vector<std::pair<int, int>>()> get_valid_positions ) {
    GameModel::PlacementRequest* request = new GameModel::PlacementRequest( type, get_valid_positions );
    _model->_queued_request.push( request );
}

#pragma endregion PaymentConfirmationState

#pragma region PostLastGenerationState

PostLastGenerationState::PostLastGenerationState( GameModel* model ) : GameModelState( model ) {}
PostLastGenerationState::~PostLastGenerationState() {}

bool PostLastGenerationState::CanConvertPlantsToGreenery() {
    return _model->_local_player->GetResource( Resource::PLANTS ) >= _model->_local_player->get_greenery_cost() &&
           _model->_board->GetValidGreeneryTiles( _model->_local_player ).size() > 0;
}

void PostLastGenerationState::ConvertPlantsToGreenery() {
    if ( !CanConvertPlantsToGreenery() )
        throw std::logic_error( "IdleState::ConvertPlantsToGreenery: cannot convert resources at this time!" );

    _model->_local_player->LoseResource( Resource::PLANTS, _model->_local_player->get_greenery_cost() );
    _model->_local_player->PlaceGreenery();
}

void PostLastGenerationState::EndTurn() {
    // TODO
}

void PostLastGenerationState::DoOnPlacementConfirmation( boards::TileType type, std::function<std::vector<std::pair<int, int>>()> get_valid_positions ) {
    GameModel::PlacementRequest* request = new GameModel::PlacementRequest( type, get_valid_positions );
    PerformRequest( request );
}

#pragma endregion PostLastGenerationState

#pragma region PostLastGenerationPlacementConfirmationState

PostLastGenerationPlacementConfirmationState::PostLastGenerationPlacementConfirmationState( GameModel* model, GameModel::PostLastGenerationGreeneryPlacementRequest* request )
    : GameModelState( model ), _request( request ) {
    if ( request == nullptr )
        throw std::logic_error( "PostLastGenerationPlacementConfirmationState::ctor: request was null!" );
}

PostLastGenerationPlacementConfirmationState::~PostLastGenerationPlacementConfirmationState() {
    delete _request;
}

void PostLastGenerationPlacementConfirmationState::TilePlacementConfirmed( int q, int r ) {
    if ( std::find( _request->valid_positions.cbegin(), _request->valid_positions.cend(), std::pair<int, int>( q, r ) ) == _request->valid_positions.cend() )
        throw std::logic_error( std::format( "PostLastGenerationPlacementConfirmationState::TilePlacementConfirmed: indices q: {}, r: {} are not valid positions!", q, r ) );

    _model->_board->PlaceTile( q, r, _model->_local_player, boards::TileType::GREENERY );

    _model->RequestStateChange( _model->CreatePostLastGenerationState() );
}

#pragma endregion PostLastGenerationPlacementConfirmationState

#pragma region GameOverState

GameOverState::GameOverState( GameModel* model ) : GameModelState( model ) {}
GameOverState::~GameOverState() {}

#pragma endregion GameOverState
}
