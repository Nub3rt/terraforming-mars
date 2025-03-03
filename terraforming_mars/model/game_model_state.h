#pragma once

#include "game_model.fwd.h"
#include "game_model_state.fwd.h"

#include "player.h"
#include "resource.h"

namespace model
{
class GameModelState
{
public:
    GameModelState() {}
    virtual ~GameModelState() {}

    virtual void PlayCard( GameModel* model, decks::Card* card );
    virtual void SellCardSP( GameModel* model, decks::Card* card );
    virtual void UsePowerPlantSP( GameModel* model );
    virtual void UseAsteroidSP( GameModel* model );
    virtual void UseAquiferSP( GameModel* model );
    virtual void UseGreenerySP( GameModel* model );
    virtual void UseCitySP( GameModel* model );

    virtual bool CanUsePowerPlantSP( GameModel* model );
    virtual bool CanUseAsteroidSP( GameModel* model );
    virtual bool CanUseAquiferSP( GameModel* model );
    virtual bool CanUseGreenerySP( GameModel* model );
    virtual bool CanUseCitySP( GameModel* model );

    virtual void OnDrawCard( GameModel* model, Player* player );
    virtual void OnRaiseTR( GameModel* model, Player* player, int amount );
    virtual void OnRaiseTemperature( GameModel* model, Player* player );
    virtual void OnPlaceOcean( GameModel* model, Player* player );
    virtual void OnPlaceOceanOnNonOcean( GameModel* model, Player* player );
    virtual void OnRaiseOxygen( GameModel* model, Player* player );
    virtual void OnPlaceGreenery( GameModel* model, Player* player );
    virtual void OnPlaceGreeneryOnOcean( GameModel* model, Player* player );
    virtual void OnPlaceCity( GameModel* model, Player* player );
    virtual void OnPlaceNoctisCity( GameModel* model, Player* player );
    virtual void OnPlaceLonelyCity( GameModel* model, Player* player );
    virtual void OnPlaceUrbanizedArea( GameModel* model, Player* player );
    virtual void OnResourceAmountChanged( GameModel* model, Player* player, Resource resource, int amount );
    virtual void OnResourceProductionAmountChanged( GameModel* model, Player* player, Resource resource, int amount );
    virtual void OnDestroyResource( GameModel* model, Player* player, Resource resource, int amount );
    virtual void OnDestroyResourceProduction( GameModel* model, Player* player, Resource resource, int amount );
};

class IdleState : public GameModelState
{

};

class PlacementConfirmationState : public GameModelState
{

};

class PaymentConfirmationState : public GameModelState
{

};
}
