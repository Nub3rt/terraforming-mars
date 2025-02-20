/* Axial coordinates explained: https://www.redblobgames.com/grids/hexagons/
 * The first coordinate is q, the second is r, and s is the third, satisfying q + r + s = 0 or s = -q-r
 *
 *            /\      /\      /\      /\      /\
 *           /  \    /  \    /  \    /  \    /  \
 *          /    \  /    \  /    \  /    \  /    \
 *         |      ||      ||      ||      ||      |
 *         | 4  0 || 5  0 || 6  0 || 7  0 || 8  0 |
 *         |      ||      ||      ||      ||      |
 *       /  \    /  \    /  \    /  \    /  \    /  \
 *      /    \  /    \  /    \  /    \  /    \  /    \
 *     |      ||      ||      ||      ||      ||      |
 *     | 3  1 || 4  1 || 5  1 || 6  1 || 7  1 || 8  1 |
 *     |      ||      ||      ||      ||      ||      |
 *   /  \    /  \    /  \    /  \    /  \    /  \    /  \
 *  /    \  /    \  /    \  /    \  /    \  /    \  /    \
 * |      ||      ||      ||      ||      ||      ||      |
 * | 2  2 || 3  2 || 4  2 || 5  2 || 6  2 || 7  2 || 8  2 |
 * |      ||      ||      ||      ||      ||      ||      |
 *  \    /  \    /  \    /  \    /  \    /  \    /  \    /
 *   \  / .  \  / .  \  / .  \  / .  \  / .  \  / .  \  / .
 *    \/   .  \/   .  \/   .  \/   .  \/   .  \/   .  \/   .
 *          .       .       .       .       .       .       .
 *
 * Indexes with a sum of <4 may be reserved for special tiles
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
    inline bool CanPlaceTile( int q, int r, Player* player, TileType type );
    inline void PlaceTile( int q, int r, Player* player, TileType type );

private:
    ConcreteBoard* _concrete_board;
};
}
