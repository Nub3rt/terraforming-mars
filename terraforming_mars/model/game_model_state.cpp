#include "game_model_state.h"

#include <stdexcept>

#include "game_model.h"
#include "player.h"
#include "resource.h"

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

void GameModelState::SellCardSP( decks::Card* card ) { throw std::logic_error( "GameModelState::SellCardSP: GameModel was in an invalid state!" ); }
void GameModelState::UsePowerPlantSP() { throw std::logic_error( "GameModelState::UsePowerPlantSP: GameModel was in an invalid state!" ); }
void GameModelState::UseAsteroidSP() { throw std::logic_error( "GameModelState::UseAsteroidSP: GameModel was in an invalid state!" ); }
void GameModelState::UseAquiferSP() { throw std::logic_error( "GameModelState::UseAquiferSP: GameModel was in an invalid state!" ); }
void GameModelState::UseGreenerySP() { throw std::logic_error( "GameModelState::UseGreenerySP: GameModel was in an invalid state!" ); }
void GameModelState::UseCitySP() { throw std::logic_error( "GameModelState::UseCitySP: GameModel was in an invalid state!" ); }

void GameModelState::PlayCard( decks::Card* card ) { throw std::logic_error( "GameModelState::PlayCard: GameModel was in an invalid state!" ); }
void GameModelState::UseAction( decks::Card* card ) { throw std::logic_error( "GameModelState::UseAction: GameModel was in an invalid state!" ); }
void GameModelState::ConvertPlantsToGreenery() { throw std::logic_error( "GameModelState::ConvertPlantsToGreenery: GameModel was in an invalid state!" ); }
void GameModelState::ConvertHeatToTemperature() { throw std::logic_error( "GameModelState::ConvertHeatToTemperature: GameModel was in an invalid state!" ); }

void GameModelState::TilePlacementConfirmed( int q, int r ) { throw std::logic_error( "GameModelState::TilePlacementConfirmed: GameModel was in an invalid state!" ); }

void GameModelState::PaymentConfirmed( int credit, int resource ) { throw std::logic_error( "GameModelState::PaymentConfirmed: GameModel was in an invalid state!" ); }

void GameModelState::EndTurn() { throw std::logic_error( "GameModelState::EndTurn: GameModel was in an invalid state!" ); }

void GameModelState::Player_OnDrawCard( Player* player ) { throw std::logic_error( "GameModelState::Player_OnDrawCard: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnRaiseTR( Player* player, int amount ) { throw std::logic_error( "GameModelState::Player_OnRaiseTR: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnRaiseTemperature( Player* player ) { throw std::logic_error( "GameModelState::Player_OnRaiseTemperature: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnPlaceOcean( Player* player ) { throw std::logic_error( "GameModelState::Player_OnPlaceOcean: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnPlaceOceanOnNonOcean( Player* player ) { throw std::logic_error( "GameModelState::Player_OnPlaceOceanOnNonOcean: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnRaiseOxygen( Player* player ) { throw std::logic_error( "GameModelState::Player_OnRaiseOxygen: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnPlaceGreenery( Player* player ) { throw std::logic_error( "GameModelState::Player_OnPlaceGreenery: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnPlaceGreeneryOnOcean( Player* player ) { throw std::logic_error( "GameModelState::Player_OnPlaceGreeneryOnOcean: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnPlaceCity( Player* player ) { throw std::logic_error( "GameModelState::Player_OnPlaceCity: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnPlaceNoctisCity( Player* player ) { throw std::logic_error( "GameModelState::Player_OnPlaceNoctisCity: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnPlaceLonelyCity( Player* player ) { throw std::logic_error( "GameModelState::Player_OnPlaceLonelyCity: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnPlaceUrbanizedArea( Player* player ) { throw std::logic_error( "GameModelState::Player_OnPlaceUrbanizedArea: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnResourceAmountChanged( Player* player, Resource resource, int amount )
    { throw std::logic_error( "GameModelState::Player_OnResourceAmountChanged: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnResourceProductionAmountChanged( Player* player, Resource resource, int amount )
    { throw std::logic_error( "GameModelState::Player_OnResourceProductionAmountChanged: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnDestroyResource( Player* player, Resource resource, int amount )
    { throw std::logic_error( "GameModelState::Player_OnDestroyResource: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnDestroyResourceProduction( Player* player, Resource resource, int amount )
    { throw std::logic_error( "GameModelState::Player_OnDestroyResourceProduction: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment )
    { throw std::logic_error( "GameModelState::Player_OnConfirmSteelPayment: GameModel was in an invalid state!" ); }
void GameModelState::Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment )
    { throw std::logic_error( "GameModelState::Player_OnConfirmTitaniumPayment: GameModel was in an invalid state!" ); }

#pragma endregion GameModelState

#pragma region IdleState

void IdleState::PerformRequest( GameModel::PaymentRequest* request ) {
}

void IdleState::PerformRequest( GameModel::PlacementRequest* request ) {
}


#pragma endregion IdleState

}
