#pragma once

#include "game_model.fwd.h"
#include "player.fwd.h"
#include "card.fwd.h"

namespace model
{
class GameModel
{
public:
    virtual ~GameModel();

    int Temperature() const;
    int OceanCount() const;
    int Oxygen() const;
    int CityCount() const;

    bool IsTilePlaceable() const;
    bool IsCityPlaceable() const;
    bool IsUrbanizedAreaPlaceable() const;
    bool IsAvailableLonelyTile() const;

protected:
    GameModel();
};
}
