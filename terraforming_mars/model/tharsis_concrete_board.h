#pragma once

#include <array>
#include <functional>

#include "concrete_board.h"
#include "hexagonal_grid.h"
#include "player.h"
#include "tile.h"

namespace model::board
{
class TharsisConcreteBoard : public ConcreteBoard
{
public:
    TharsisConcreteBoard() noexcept;
    ~TharsisConcreteBoard() noexcept;

    inline const Tile& operator()( int q, int r ) const noexcept override;

protected:
    inline Tile& get_tile( int q, int r ) noexcept override;
    std::vector<std::reference_wrapper<const Tile>> GetNeighbours( int q, int r ) const noexcept override;

private:
    std::array<std::array<Tile, 9>, 9> _board;

    static const std::array<std::array<Tile, 9>, 9> _starting_board;
};
}
