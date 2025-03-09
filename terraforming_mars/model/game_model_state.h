#pragma once

#include "game_model.fwd.h"
#include "game_model_state.fwd.h"

#include <utility>
#include <vector>

#include "game_model.h"
#include "player.h"
#include "resource.h"

namespace model
{
class GameModelState
{
    using pii = std::pair<int, int>;

public:
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

    virtual void PlayCard( decks::Card* card );
    virtual void UseAction( decks::Card* card );
    virtual void ConvertPlantsToGreenery();
    virtual void ConvertHeatToTemperature();

    virtual void TilePlacementConfirmed( int q, int r );

    virtual void PaymentConfirmed( int credit, int resource );

    virtual void EndTurn();

    virtual void PerformRequest( GameModel::PaymentRequest* request ) = 0;
    virtual void PerformRequest( GameModel::PlacementRequest* request ) = 0;

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

protected:
    GameModelState( GameModel* model );
    GameModel* _model;
};

class IdleState : public GameModelState
{
public:
    IdleState( GameModel* model);
    ~IdleState();

    bool CanUsePowerPlantSP() override;
    bool CanUseAsteroidSP() override;
    bool CanUseAquiferSP() override;
    bool CanUseGreenerySP() override;
    bool CanUseCitySP() override;

    void PerformRequest( GameModel::PaymentRequest* request ) override;
    void PerformRequest( GameModel::PlacementRequest* request ) override;
};

class PlacementConfirmationState : public GameModelState
{
public:
    PlacementConfirmationState();
    ~PlacementConfirmationState();
};

class PaymentConfirmationState : public GameModelState
{

};
}
