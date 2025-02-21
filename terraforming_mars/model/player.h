#pragma once

#include "player.fwd.h"
#include "card.fwd.h"

#include "tag.h"

namespace model
{
class Player
{
public:
    Player();
    ~Player();

    int get_credits() const;

    int GetMaxPayAmountForBuilding() const;
    int GetMaxPayAmountForSpace() const;

    void DrawCard();
    void GainCredits( int delta );
    void LoseCredits( int delta );
    void GainSteel( int delta );
    void GainTitanium( int delta );
    void GainPlants( int delta );

    void PlayCard( decks::Card* card ); // this gets called in Card
    int CalculateCardCost( int base_cost );

private:
    // void AddTag( decks::Tag tag );
};
}
