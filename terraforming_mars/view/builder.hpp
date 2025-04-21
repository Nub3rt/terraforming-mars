#pragma once

#include <random>

#include "gl_utils/camera.hpp"
#include "game_view.hpp"

#include "../model/game_model.hpp"
#include "../model/boards/concrete_board.hpp"
#include "../model/decks/deck_provider.hpp"

namespace view
{
class Builder
{
public:
    Builder();
    ~Builder();

    Builder& SetSeed( unsigned int seed );

    Builder& SoloGameModel();
    Builder& TharsisBoard();
    Builder& ReducedBasicDeck();

    Builder& SoloGameView();

    GameView* GetResult( Camera* camera );

protected:
    std::mt19937 _random;

    model::GameModel* _model = nullptr;
    model::boards::ConcreteBoard* _concrete_board = nullptr;
    model::decks::DeckProvider* _deck_provider = nullptr;

    GameView* _view = nullptr;
};
}