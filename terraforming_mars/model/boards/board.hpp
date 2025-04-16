/* Axial coordinates explained: https://www.redblobgames.com/grids/hexagons/
 * The first coordinate is q, the second is r, and s is the third, satisfying q + r + s = 0 or s = -q-r
 *
 *       /\           /\      /\      /\      /\      /\
 *      /  \         /  \    /  \    /  \    /  \    /  \
 *     /    \       /    \  /    \  /    \  /    \  /    \
 *    | q  r |     | 4  0 || 5  0 || 6  0 || 7  0 || 8  0 |
 *    |      |     |      ||      ||      ||      ||      |
 *    |   s  |     |  -4  ||  -5  ||  -6  ||  -7  ||  -8  |
 *     \    /    /  \    /  \    /  \    /  \    /  \    /  \
 *      \  /    /    \  /    \  /    \  /    \  /    \  /    \
 *       \/    | 3  1 || 4  1 || 5  1 || 6  1 || 7  1 || 8  1 |
 *             |      ||      ||      ||      ||      ||      |
 *             |  -4  ||  -5  ||  -6  ||  -7  ||  -8  ||  -9  |
 *           /  \    /  \    /  \    /  \    /  \    /  \    /  \
 *          /    \  /    \  /    \  /    \  /    \  /    \  /    \
 *         | 2  2 || 3  2 || 4  2 || 5  2 || 6  2 || 7  2 || 8  2 |
 *         |      ||      ||      ||      ||      ||      ||      |
 *         |  -4  ||  -5  ||  -6  ||  -7  ||  -8  ||  -9  ||  -10 |
 *       /  \    /  \    /  \    /  \    /  \    /  \    /  \    /  \
 *      /    \  /    \  /    \  /    \  /    \  /    \  /    \  /    \
 *     | 1  3 || 2  3 || 3  3 || 4  3 || 5  3 || 6  3 || 7  3 || 8  3 |
 *     |      ||      ||      ||      ||      ||      ||      ||      |
 *     |  -4  ||  -5  ||  -6  ||  -7  ||  -8  ||  -9  ||  -10 ||  -11 |
 *   /  \    /  \    /  \    /  \    /  \    /  \    /  \    /  \    /  \
 *  /    \  /    \  /    \  /    \  /    \  /    \  /    \  /    \  /    \
 * | 0  4 || 1  4 || 2  4 || 3  4 || 4  4 || 5  4 || 6  4 || 7  4 || 8  4 |
 * |      ||      ||      ||      ||      ||      ||      ||      ||      |
 * |  -4  ||  -5  ||  -6  ||  -7  ||  -8  ||  -9  ||  -10 ||  -11 ||  -12 |
 *  \    /  \    /  \    /  \    /  \    /  \    /  \    /  \    /  \    /
 *   \  /    \  /    \  /    \  /    \  /    \  /    \  /    \  /    \  /
 *     | 0  5 || 1  5 || 2  5 || 3  5 || 4  5 || 5  5 || 6  5 || 7  5 |
 *     |      ||      ||      ||      ||      ||      ||      ||      |
 *     |  -5  ||  -6  ||  -7  ||  -8  ||  -9  ||  -10 ||  -11 ||  -12 |
 *      \    /  \    /  \    /  \    /  \    /  \    /  \    /  \    /
 *       \  /    \  /    \  /    \  /    \  /    \  /    \  /    \  /
 *         | 0  6 || 1  6 || 2  6 || 3  6 || 4  6 || 5  6 || 6  6 |
 *         |      ||      ||      ||      ||      ||      ||      |
 *         |  -6  ||  -7  ||  -8  ||  -9  ||  -10 ||  -11 ||  -12 |
 *          \    /  \    /  \    /  \    /  \    /  \    /  \    /
 *           \  /    \  /    \  /    \  /    \  /    \  /    \  /
 *             | 0  7 || 1  7 || 2  7 || 3  7 || 4  7 || 5  7 |
 *             |      ||      ||      ||      ||      ||      |
 *             |  -7  ||  -8  ||  -9  ||  -10 ||  -11 ||  -12 |
 *              \    /  \    /  \    /  \    /  \    /  \    /
 *               \  /    \  /    \  /    \  /    \  /    \  /
 *                 | 0  8 || 1  8 || 2  8 || 3  8 || 4  8 |
 5                 |      ||      ||      ||      ||      |
 *                 |  -8  ||  -9  ||  -10 ||  -11 ||  -12 |
 *                  \    /  \    /  \    /  \    /  \    /
 *                   \  /    \  /    \  /    \  /    \  /
 *                    \/      \/      \/      \/      \/
 * 
 * Indexes with q + r < 4 may be reserved for special tiles
 */

#pragma once

#include <utility>
#include <vector>

#include "concrete_board.hpp"
#include "../player.hpp"
#include "tile.hpp"
#include "tile_type.hpp"

namespace model::boards
{
class Board
{
public:
    Board( ConcreteBoard* board ) noexcept;
    ~Board() noexcept;
    Board( const Board& other ) = delete;
    Board( Board&& other ) = delete;
    Board& operator=( const Board& other ) = delete;
    Board& operator=( Board&& other ) = delete;

    inline const Tile& operator()( int q, int r ) const noexcept { return _concrete_board->get_tile( q, r ); }
    inline const Tile& get_tile( int q, int r ) const noexcept { return _concrete_board->get_tile( q, r ); }

    inline std::vector<std::pair<int, int>> GetNeighbouringTiles( int q, int r ) const { return _concrete_board->GetNeighbouringTiles( q, r ); }
    inline std::vector<std::pair<int, int>> GetNeighbouringTilesOfType( int q, int r, TileType type ) const { return _concrete_board->GetNeighbouringTilesOfType( q, r, type ); }
    inline std::vector<std::pair<int, int>> GetNeighbouringTilesOfTypeRange( int q, int r, TileType min, TileType max ) const { return _concrete_board->GetNeighbouringTilesOfTypeRange( q, r, min, max ); }
    std::vector<std::pair<int, int>> GetTilesOfType( TileType type ) const;
    std::vector<std::pair<int, int>> GetPlaceableTilesOfType( const Player* player, TileType type ) const;
    std::vector<std::pair<int, int>> GetEmptyTiles( const Player* player ) const;
    std::vector<std::pair<int, int>> GetValidOceanTiles( const Player* player) const;
    std::vector<std::pair<int, int>> GetValidGreeneryTiles( const Player* player ) const;
    std::vector<std::pair<int, int>> GetValidCityTiles( const Player* player ) const;
    std::vector<std::pair<int, int>> GetValidNoctisCityTiles( const Player* player ) const;
    std::vector<std::pair<int, int>> GetValidLonelyCityTiles( const Player* player ) const;
    std::vector<std::pair<int, int>> GetValidUrbanizedAreaTiles( const Player* player ) const;

    inline void PlaceTile( int q, int r, Player* player, TileType type ) { _concrete_board->PlaceTile( q, r, player, type ); }
    inline void SetOwner( int q, int r, Player* player ) { _concrete_board->SetOwner( q, r, player ); }
    inline void SetTileType( int q, int r, TileType type ) { _concrete_board->SetTileType( q, r, type ); }

    inline void SetOnTilePlacedCallback( std::function<void( int, int, const Tile& )> callback ) { _concrete_board->SetOnTilePlacedCallback( callback ); }

    inline ConcreteBoard::IteratorWrapper begin() const { return _concrete_board->begin(); }
    inline ConcreteBoard::IteratorWrapper end() const { return _concrete_board->end(); }

private:
    ConcreteBoard* _concrete_board;
};
}
