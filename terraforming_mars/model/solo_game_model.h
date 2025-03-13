#pragma once

#include "game_model.h"

#include "board.h"
#include "deck.h"
#include "player.h"

namespace model
{
class SoloGameModel : public GameModel
{
public:
    SoloGameModel( int seed );
    ~SoloGameModel();

    void Initialize( board::Board* board, decks::Deck* deck ) override;

protected:
    Player* CreateLocalPlayer() override;

    void Player_OnDestroyResource( Player* player, Resource resource, int amount ) override;
    void Player_OnDestroyResourceProduction( Player* player, Resource resource, int amount ) override;
};
}
