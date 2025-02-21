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

#include "concrete_board.h"
#include "player.h"
#include "tile.h"
#include "tile_type.h"

namespace model::board
{
class Board
{
public:
    Board( ConcreteBoard* board ) noexcept;
    ~Board() noexcept;

    inline const Tile& operator()( int q, int r ) const noexcept;
    inline bool CanPlaceTile( int q, int r, const Player* player, TileType type ) const;
    inline void PlaceTile( int q, int r, Player* player, TileType type );
    int NeighbouringTiles( int q, int r ) const;
    int NeighbouringTilesOfType( int q, int r, TileType type ) const;

private:
    ConcreteBoard* _concrete_board;
};
}
