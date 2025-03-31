#include "builder.h"

#include <stdexcept>

#include "gl_utils/camera.h"
#include "view.h"

#include "../model/game_model.h"
#include "../model/solo_game_model.h"
#include "../model/boards/tharsis_concrete_board.h"
#include "../model/decks/deck.h"
#include "../model/decks/deck_provider.h"
#include "../model/decks/reduced_basic_deck_provider.h"

namespace view
{
Builder::Builder( int seed ) : _random( seed ) {}

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

    _view = new View();

    return *this;
}

View* Builder::GetResult( Camera* camera ) {
    if ( _model == nullptr || _view == nullptr ||
         _concrete_board == nullptr || _deck_provider == nullptr ) {
        throw std::logic_error( "Builder::GetResult: some components missing!" );
    }

    model::boards::Board* board = new model::boards::Board( _concrete_board );
    model::decks::Deck* deck = new model::decks::Deck( *_model, *_deck_provider, _random()  );
    _model->Initialize( board, deck );
    _view->Init( camera, _model );

    delete _deck_provider;

    View* result = _view;
    _concrete_board = nullptr;
    _deck_provider = nullptr;
    _model = nullptr;
    _view = nullptr;

    return result;
}
}
