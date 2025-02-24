#pragma once

#include "player.fwd.h"
#include "card.fwd.h"

#include <functional>

#include "resource.h"
#include "tag.h"

namespace model
{
class Player
{
public:
    Player();
    ~Player();

    int GetResource( Resource resource ) const;
    int GetResourceProduction( Resource resource) const;

    void GainResource( Resource resource, int amount );
    void GainResourceProduction( Resource resource, int amount );

    void LoseResource( Resource resource, int amount );
    void LoseResourceProduction( Resource resource, int amount );

    void DestroyResource( Resource resource, int amount );
    void DestroyResourceProduction( Resource resource, int amount );

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

    int GetTagCount( Tag tag );

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
