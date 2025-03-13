#pragma once

#include "game_model.fwd.h"
#include "game_model_state.fwd.h"

#include <utility>
#include <vector>

#include "card.h"
#include "active_card_with_action.h"
#include "game_model.h"
#include "player.h"
#include "resource.h"

namespace model
{
class GameModelState
{
public:
    using pii = std::pair<int, int>;

    virtual ~GameModelState() {}

    virtual bool CanUsePowerPlantSP();
    virtual bool CanUseAsteroidSP();
    virtual bool CanUseAquiferSP();
    virtual bool CanUseGreenerySP();
    virtual bool CanUseCitySP();
    virtual bool CanConvertPlantsToGreenery();
    virtual bool CanConvertHeatToTemperature();

    virtual bool InIdleState();

    virtual void SellCardSP( decks::Card* card );
    virtual void UsePowerPlantSP();
    virtual void UseAsteroidSP();
    virtual void UseAquiferSP();
    virtual void UseGreenerySP();
    virtual void UseCitySP();
    virtual void ConvertPlantsToGreenery();
    virtual void ConvertHeatToTemperature();

    virtual void PlayCard( decks::Card* card );
    virtual void UseAction( decks::ActiveCardWithAction* card );

    virtual void TilePlacementConfirmed( int q, int r );

    virtual void PaymentConfirmed( int credit, int resource );

    virtual void EndTurn();

    void PerformRequest( GameModel::PlacementRequest* request );
    void PerformRequest( GameModel::PaymentRequest* request );
    void PerformRequest( GameModel::PostLastGenerationGreeneryPlacementRequest* request );

    virtual void Player_OnDrawCard( Player* player );
    virtual void Player_OnRaiseTR( Player* player, int amount );
    virtual void Player_OnRaiseTemperature( Player* player );
    virtual void Player_OnRaiseOxygen( Player* player );

    void Player_OnPlaceOcean( Player* player );
    void Player_OnPlaceOceanOnNonOcean( Player* player );
    void Player_OnPlaceGreenery( Player* player );
    void Player_OnPlaceGreeneryOnOcean( Player* player );
    void Player_OnPlaceCity( Player* player );
    void Player_OnPlaceNoctisCity( Player* player );
    void Player_OnPlaceLonelyCity( Player* player );
    void Player_OnPlaceUrbanizedArea( Player* player );

    virtual void Player_OnResourceAmountChanged( Player* player, Resource resource, int amount );
    virtual void Player_OnResourceProductionAmountChanged( Player* player, Resource resource, int amount );
    virtual void Player_OnDestroyResource( Player* player, Resource resource, int amount );
    virtual void Player_OnDestroyResourceProduction( Player* player, Resource resource, int amount );

    virtual void Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment ) = 0;
    virtual void Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment ) = 0;

protected:
    GameModelState( GameModel* model );

    GameModel* _model;

    virtual void DoOnPlacementConfirmation( board::TileType type, std::vector<pii> valid_positions ) = 0;
};

class IdleState : public GameModelState
{
public:
    IdleState( GameModel* model );
    ~IdleState();

    bool CanUsePowerPlantSP() override;
    bool CanUseAsteroidSP() override;
    bool CanUseAquiferSP() override;
    bool CanUseGreenerySP() override;
    bool CanUseCitySP() override;
    bool CanConvertPlantsToGreenery() override;
    bool CanConvertHeatToTemperature() override;

    bool InIdleState() override;

    void SellCardSP( decks::Card* card ) override;
    void UsePowerPlantSP() override;
    void UseAsteroidSP() override;
    void UseAquiferSP() override;
    void UseGreenerySP() override;
    void UseCitySP() override;

    void PlayCard( decks::Card* card ) override;
    void UseAction( decks::ActiveCardWithAction* card ) override;
    void ConvertPlantsToGreenery() override;
    void ConvertHeatToTemperature() override;

    void EndTurn() override;

    void Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment ) override;
    void Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment ) override;

protected:
    void DoOnPlacementConfirmation( board::TileType type, std::vector<pii> valid_positions ) override;
};

class PlacementConfirmationState : public GameModelState
{
public:
    PlacementConfirmationState( GameModel* model, GameModel::PlacementRequest* request );
    ~PlacementConfirmationState();

    void TilePlacementConfirmed( int q, int r ) override;

    void Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment ) override;
    void Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment ) override;

protected:
    void DoOnPlacementConfirmation( board::TileType type, std::vector<pii> valid_positions ) override;

    GameModel::PlacementRequest* _request;
};

class PaymentConfirmationState : public GameModelState
{
public:
    PaymentConfirmationState( GameModel* model, GameModel::PaymentRequest* request );
    ~PaymentConfirmationState();

    void PaymentConfirmed( int credit, int resource ) override;

    void Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment ) override;
    void Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment ) override;

protected:
    void DoOnPlacementConfirmation( board::TileType type, std::vector<pii> valid_positions ) override;

    GameModel::PaymentRequest* _request;
};

class PostLastGenerationState : public GameModelState
{
public:
    PostLastGenerationState( GameModel* model );
    ~PostLastGenerationState();

    bool CanConvertPlantsToGreenery() override;

    virtual void ConvertPlantsToGreenery();

    void EndTurn() override;

    void Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment ) override;
    void Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment ) override;

protected:
    void DoOnPlacementConfirmation( board::TileType type, std::vector<pii> valid_positions ) override;
};

class PostLastGenerationPlacementConfirmationState : public GameModelState
{
public:
    PostLastGenerationPlacementConfirmationState( GameModel* model, GameModel::PostLastGenerationGreeneryPlacementRequest* request );
    ~PostLastGenerationPlacementConfirmationState();

    void TilePlacementConfirmed( int q, int r ) override;

    void Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment ) override;
    void Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment ) override;

    void DoOnPlacementConfirmation( board::TileType type, std::vector<pii> valid_positions ) override;

    GameModel::PostLastGenerationGreeneryPlacementRequest* _request;
};

class GameOverState : public GameModelState
{
public:
    GameOverState( GameModel* model );
    ~GameOverState();

    void Player_OnConfirmSteelPayment( Player* player, int cost, std::function<void()> after_payment ) override;
    void Player_OnConfirmTitaniumPayment( Player* player, int cost, std::function<void()> after_payment ) override;

protected:
    void DoOnPlacementConfirmation( board::TileType type, std::vector<pii> valid_positions ) override;
};
}
