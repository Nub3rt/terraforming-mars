#include "board.h"

#include <utility>
#include <vector>

namespace model::board
{
using pii = std::pair<int, int>;

Board::Board( ConcreteBoard* board ) noexcept : _concrete_board( board ) {}

Board::~Board() noexcept {
    delete _concrete_board;
}

inline const Tile& Board::operator()( int q, int r ) const noexcept {
    return _concrete_board->get_tile( q, r );
}

inline std::vector<pii> Board::GetNeighbouringTiles( int q, int r ) const {
    return _concrete_board->GetNeighbouringTiles( q, r );
}

inline std::vector<pii> Board::GetNeighbouringTilesOfType( int q, int r, TileType type ) const {
    return _concrete_board->GetNeighbouringTilesOfType( q, r, type );
}

inline std::vector<pii> Board::GetNeighbouringTilesOfTypeRange( int q, int r, TileType min, TileType max ) const {
    return _concrete_board->GetNeighbouringTilesOfTypeRange( q, r, min, max );
}

inline void Board::PlaceTile( int q, int r, Player* player, TileType type ) {
    _concrete_board->PlaceTile( q, r, player, type );
}

inline void Board::SetOwner( int q, int r, Player* player ) {
    _concrete_board->SetOwner( q, r, player );
}

inline void Board::SetTileType( int q, int r, TileType type ) {
    _concrete_board->SetTileType( q, r, type );
}
inline void Board::SetOnTilePlacedCallback( std::function<void( int, int, const Tile& )> callback ) {
    _concrete_board->SetOnTilePlacedCallback( callback );
}
}
