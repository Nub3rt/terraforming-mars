#pragma once

#include "../model/boards/tile.h"

namespace view
{
class TileWrapper
{
public:
    TileWrapper( const model::boards::Tile& tile );
    ~TileWrapper();

    inline model::boards::Tile* operator->() noexcept { return &_tile; }
    inline model::boards::Tile& operator*() noexcept { return _tile; }

private:
    model::boards::Tile _tile;
};
}
