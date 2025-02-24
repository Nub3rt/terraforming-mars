#include "tharsis_concrete_board.h"

#include <array>
#include <functional>

#include "concrete_board.h"
#include "hexagonal_grid.h"
#include "player.h"
#include "tile.h"

namespace model::board
{
TharsisConcreteBoard::TharsisConcreteBoard() noexcept : _board( _starting_board ) {}

TharsisConcreteBoard::~TharsisConcreteBoard() noexcept {}

const Tile& TharsisConcreteBoard::operator()( int q, int r ) const noexcept {
    return _board[ r ][ q ];
}

Tile& TharsisConcreteBoard::get_tile( int q, int r ) noexcept {
    return _board[ r ][ q ];
}

std::vector<std::reference_wrapper<const Tile>> model::board::TharsisConcreteBoard::GetNeighbours( int q, int r ) const noexcept {
    std::vector<std::reference_wrapper<const Tile>> neighbours;

    int s = -q-r;
    if ( r > 0 && q < 8 )
        neighbours.push_back( _board[ r - 1 ][ q + 1 ] );
    if ( q < 8 && s > -12 )
        neighbours.push_back( _board[ r ][ q + 1 ] );
    if ( s > -12 && r < 8 )
        neighbours.push_back( _board[ r + 1 ][ q ] );
    if ( r < 8 && q > 0 )
        neighbours.push_back( _board[ r + 1 ][ q - 1 ] );
    if ( q > 0 && s < -4 )
        neighbours.push_back( _board[ r ][ q - 1 ] );
    if ( s < -4 && r > 0 )
        neighbours.push_back( _board[ r - 1 ][ q ] );

    return neighbours;
}

const std::array<std::array<Tile, 9>, 9> TharsisConcreteBoard::_starting_board = {{
    {{
        Tile( _warn_bad_index ),
        Tile( _warn_bad_index ),
        Tile( _warn_bad_index ),
        Tile( _warn_bad_index ),
        Tile( _gain_two_steel, TileType::EMPTY ),
        Tile( _gain_two_steel, TileType::RESERVED_FOR_OCEAN ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _draw_one_card, TileType::RESERVED_FOR_OCEAN ),
        Tile( _noop, TileType::RESERVED_FOR_OCEAN ),
    }}, // first row
    {{
        Tile( _warn_bad_index ),
        Tile( _warn_bad_index ),
        Tile( _warn_bad_index ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _gain_one_steel, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _draw_two_cards, TileType::RESERVED_FOR_OCEAN ),
    }}, // second row
    {{
        Tile( _warn_bad_index ),
        Tile( _warn_bad_index ),
        Tile( _draw_one_card, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _gain_one_steel, TileType::EMPTY ),
    }}, // third row
    {{
        Tile( _warn_bad_index ),
        Tile( _gain_titanium_and_plants, TileType::EMPTY),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_two_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_two_plants, TileType::RESERVED_FOR_OCEAN ),
    }}, // fourth row
    {{
        Tile( _gain_two_plants, TileType::EMPTY ),
        Tile( _gain_two_plants, TileType::EMPTY ),
        Tile( _gain_two_plants, TileType::RESERVED_FOR_NOCTIS ),
        Tile( _gain_two_plants, TileType::RESERVED_FOR_OCEAN ),
        Tile( _gain_two_plants, TileType::RESERVED_FOR_OCEAN ),
        Tile( _gain_two_plants, TileType::RESERVED_FOR_OCEAN ),
        Tile( _gain_two_plants, TileType::EMPTY ),
        Tile( _gain_two_plants, TileType::EMPTY ),
        Tile( _gain_two_plants, TileType::EMPTY ),
    }}, // fifth row
    {{
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_two_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::RESERVED_FOR_OCEAN ),
        Tile( _gain_one_plants, TileType::RESERVED_FOR_OCEAN ),
        Tile( _gain_one_plants, TileType::RESERVED_FOR_OCEAN ),
        Tile( _warn_bad_index ),
    }}, // sixth row
    {{
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _warn_bad_index ),
        Tile( _warn_bad_index ),
    }}, // seventh row
    {{
        Tile( _gain_two_steel, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _draw_one_card, TileType::EMPTY ),
        Tile( _draw_one_card, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _gain_one_titanium, TileType::EMPTY ),
        Tile( _warn_bad_index ),
        Tile( _warn_bad_index ),
        Tile( _warn_bad_index ),
    }}, // eighth row
    {{
        Tile( _gain_one_steel, TileType::EMPTY ),
        Tile( _gain_two_steel, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _gain_two_titanium, TileType::RESERVED_FOR_OCEAN ),
        Tile( _warn_bad_index ),
        Tile( _warn_bad_index ),
        Tile( _warn_bad_index ),
        Tile( _warn_bad_index ),
    }}, // ninth row
}};
}
