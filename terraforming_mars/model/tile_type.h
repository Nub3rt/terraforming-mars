#pragma once

namespace model::board
{
enum TileType
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
}
