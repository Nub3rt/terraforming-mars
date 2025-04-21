#pragma once


#include "boards/board.hpp"
#include "decks/deck.hpp"
#include "game_model.hpp"
#include "game_model_state.hpp"
#include "player.hpp"

namespace model
{
class SoloGameModel : public GameModel
{
public:
    SoloGameModel( int seed );
    ~SoloGameModel();

    void Initialize( boards::Board* board, decks::Deck* deck ) override;
    void Start() override;

protected:
    Player* CreateLocalPlayer() override;

    void Player_OnDestroyResource( Player* player, Resource resource, int amount ) override;
    void Player_OnDestroyResourceProduction( Player* player, Resource resource, int amount ) override;

    IdleState* CreateIdleState() override;
    PostLastGenerationState* CreatePostLastGenerationState() override;

    void OnAnyoneEffect( std::function<void( decks::ActiveCardWithEffect* )> effect ) override;
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
