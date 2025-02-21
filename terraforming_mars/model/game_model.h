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

protected:
    GameModel();
};
}
