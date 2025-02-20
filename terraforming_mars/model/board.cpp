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

inline bool Board::CanPlaceTile( int q, int r, Player* player, TileType type ) {
    return _concrete_board->CanPlaceTile( q, r, player, type );
}

inline void Board::PlaceTile( int q, int r, Player* player, TileType type ) {
    _concrete_board->PlaceTile( q, r, player, type );
}
}
