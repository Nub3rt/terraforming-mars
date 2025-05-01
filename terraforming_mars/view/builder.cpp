#include "builder.hpp"

#include <stdexcept>

#include "gl_utils/camera.hpp"
#include "game_view.hpp"
#include "solo_game_view.hpp"

#include "../model/game_model.hpp"
#include "../model/solo_game_model.hpp"
#include "../model/boards/tharsis_concrete_board.hpp"
#include "../model/decks/deck.hpp"
#include "../model/decks/deck_provider.hpp"
#include "../model/decks/reduced_basic_deck_provider.hpp"

namespace view
{
Builder::Builder() : _random() {}

Builder::~Builder() {
    if ( _model != nullptr )
        delete _model;
    if ( _concrete_board != nullptr )
        delete _concrete_board;
    if ( _deck_provider != nullptr )
        delete _deck_provider;
    if ( _view != nullptr )
        delete _view;
}

Builder& Builder::SetSeed( unsigned int seed ) {
    _random.seed( seed );

    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Seed of the game: %d", seed );

    return *this;
}

Builder& Builder::SoloGameModel() {
    if ( _model != nullptr )
        delete _model;

    _model = new model::SoloGameModel( _random() );

    return *this;
}

Builder& Builder::TharsisBoard() {
    if ( _concrete_board != nullptr )
        delete _concrete_board;

    _concrete_board = new model::boards::TharsisConcreteBoard();

    return *this;
}

Builder& Builder::ReducedBasicDeck() {
    if ( _deck_provider != nullptr )
        delete _deck_provider;

    _deck_provider = new model::decks::ReducedBasicDeckProvider();

    return *this;
}

Builder& Builder::SoloGameView() {
    if ( _view != nullptr )
        delete _view;

    _view = new view::SoloGameView( _random() );

    return *this;
}

GameView* Builder::GetResult( Camera* camera ) {
    if ( _model == nullptr || _view == nullptr ||
         _concrete_board == nullptr || _deck_provider == nullptr ) {
        throw std::logic_error( "Builder::GetResult: some components missing!" );
    }

    model::boards::Board* board = new model::boards::Board( _concrete_board );
    model::decks::Deck* deck = new model::decks::Deck( *_model, *_deck_provider, _random()  );
    _model->Initialize( board, deck );
    _view->Init( camera, _model );

    delete _deck_provider;

    GameView* result = _view;
    _concrete_board = nullptr;
    _deck_provider = nullptr;
    _model = nullptr;
    _view = nullptr;

    return result;
}
}
