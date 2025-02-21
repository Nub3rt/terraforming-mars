#include "board.h"

namespace model::board
{
Board::Board( ConcreteBoard* board ) noexcept : _concrete_board(board) {}

Board::~Board() noexcept {
    delete _concrete_board;
}

inline const Tile& Board::operator()( int q, int r ) const noexcept {
    return _concrete_board->operator()( q, r );
}

inline bool Board::CanPlaceTile( int q, int r, const Player* player, TileType type ) const {
    return _concrete_board->CanPlaceTile( q, r, player, type );
}

inline void Board::PlaceTile( int q, int r, Player* player, TileType type ) {
    _concrete_board->PlaceTile( q, r, player, type );
}
int Board::NeighbouringTiles( int q, int r ) const {
    return _concrete_board->NeighbouringTiles( q, r );
}

int Board::NeighbouringTilesOfType( int q, int r, TileType type ) const {
    return _concrete_board->NeighbouringTilesOfType( q, r, type );
}
}
