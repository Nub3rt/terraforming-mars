#pragma once


#include "board.h"
#include "deck.h"
#include "game_model.h"
#include "game_model_state.h"
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

    IdleState* CreateIdleState() override;
    PostLastGenerationState* CreatePostLastGenerationState() override;
};


class SoloIdleState : public IdleState
{
public:
    SoloIdleState( GameModel* model );
    ~SoloIdleState();

    void EndTurn() override;
};

class SoloPostLastGenerationState : public PostLastGenerationState
{
public:
    SoloPostLastGenerationState( GameModel* model );
    ~SoloPostLastGenerationState();

    void EndTurn() override;
};
}
