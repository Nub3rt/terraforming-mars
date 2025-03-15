#pragma once

namespace model::boards
{
enum class TileType
{
    NONE = 0,

    EMPTY,
    RESERVED_FOR_OCEAN,
    RESERVED_FOR_NOCTIS,
    EMPTY_MIN = EMPTY,
    EMPTY_MAX = RESERVED_FOR_NOCTIS,

    OCEAN,
    GREENERY,
    CITY,
    PLACED_MIN = OCEAN,
    PLACED_MAX = CITY,
};

constexpr inline int operator+( TileType type ) {
    return static_cast<int>( type );
}
}
