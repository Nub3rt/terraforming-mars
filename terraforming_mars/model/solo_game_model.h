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

    Player* CreateLocalPlayer() override;
};
}
