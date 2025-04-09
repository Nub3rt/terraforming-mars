#pragma once

#include "animatable.h"

#include "../model/boards/tile.h"
#include "../model/boards/tile_type.h"

namespace view
{
class TileWrapper
{
public:
    TileWrapper( const model::boards::Tile& tile );
    ~TileWrapper();

    inline const model::boards::Tile* operator->() noexcept { return &_tile; }
    inline const model::boards::Tile& operator*() noexcept { return _tile; }

    void OnTilePlaced();
    void Update( float delta );

    Animatable<glm::vec3> color;

private:
    const model::boards::Tile& _tile;

    static glm::vec3 GetColorForTileType( model::boards::TileType tile_type );
};
}
