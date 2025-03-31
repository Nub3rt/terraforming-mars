#pragma once

#include <random>

#include "gl_utils/camera.h"
#include "view.h"

#include "../model/game_model.h"
#include "../model/boards/concrete_board.h"
#include "../model/decks/deck_provider.h"

namespace view
{
class Builder
{
public:
    Builder( int seed );
    ~Builder();

    Builder& SoloGameModel();
    Builder& TharsisBoard();
    Builder& ReducedBasicDeck();

    Builder& SoloGameView();

    View* GetResult( Camera* camera );

protected:
    std::mt19937 _random;

    model::GameModel* _model = nullptr;
    model::boards::ConcreteBoard* _concrete_board = nullptr;
    model::decks::DeckProvider* _deck_provider = nullptr;

    View* _view = nullptr;
};
}