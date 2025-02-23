#pragma once

#include "player.fwd.h"
#include "card.fwd.h"

#include <functional>

#include "tag.h"

namespace model
{
class Player
{
public:
    Player();
    ~Player();

    int get_credit() const;
    int get_steel() const;
    int get_titanium() const;
    int get_plants() const;
    int get_energy() const;
    int get_heat() const;

    int get_credit_production() const;
    int get_steel_production() const;
    int get_titanium_production() const;
    int get_plants_production() const;
    int get_energy_production() const;
    int get_heat_production() const;

    void GainCredit( int amount );
    void GainSteel( int amount );
    void GainTitanium( int amount );
    void GainPlants( int amount );
    void GainEnergy( int amount );
    void GainHeat( int amount );

    void GainCreditProduction( int amount );
    void GainSteelProduction( int amount );
    void GainTitaniumProduction( int amount );
    void GainPlantsProduction( int amount );
    void GainEnergyProduction( int amount );
    void GainHeatProduction( int amount );

    void LoseCredit( int amount );
    void LoseSteel( int amount );
    void LoseTitanium( int amount );
    void LosePlants( int amount );
    void LoseEnergy( int amount );
    void LoseHeat( int amount );

    void LoseCreditProduction( int amount );
    void LoseSteelProduction( int amount );
    void LoseTitaniumProduction( int amount );
    void LosePlantsProduction( int amount );
    void LoseEnergyProduction( int amount );
    void LoseHeatProduction( int amount );

    void DestroyCredit( int amount );
    void DestroySteel( int amount );
    void DestroyTitanium( int amount );
    void DestroyPlants( int amount );
    void DestroyEnergy( int amount );
    void DestroyHeat( int amount );

    void DestroyCreditProduction( int amount );
    void DestroySteelProduction( int amount );
    void DestroyTitaniumProduction( int amount );
    void DestroyPlantsProduction( int amount );
    void DestroyEnergyProduction( int amount );
    void DestroyHeatProduction( int amount );

    void DrawCard();

    void RaiseTR( int amount );

    void RaiseTemperature();
    void PlaceOcean();
    void PlaceOceanOnNonOcean();
    void RaiseOxygen();

    void PlaceGreenery();
    void PlaceGreeneryOnOcean();
    void PlaceCity();
    void PlaceNoctisCity();
    void PlaceLonelyCity();
    void PlaceUrbanizedArea();

    int GetTagCount( decks::Tag tag );

    int GetMaxPayAmountForBuilding() const;
    int GetMaxPayAmountForSpace() const;
    void ConfirmSteelPayment( int cost, std::function<void()> after_payment );
    void ConfirmTitaniumPayment( int cost, std::function<void()> after_payment );

    void PlayCard( decks::Card* card ); // this gets called in Card
    int CalculateCardCost( int base_cost );

private:
    // void AddTag( decks::Tag tag );
};
}
