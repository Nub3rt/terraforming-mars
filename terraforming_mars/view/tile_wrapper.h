#pragma once

#include "../model/boards/tile.h"

namespace view
{
class TileWrapper
{
public:
    TileWrapper( const model::boards::Tile& tile );
    ~TileWrapper();

    const model::boards::Tile& tile;
};
}
