#pragma once

#include "animatable.hpp"

#include "../model/boards/tile.hpp"
#include "../model/boards/tile_type.hpp"

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

    bool selectable = false;
    bool show_resources;
    Animatable<glm::vec3> color;
    glm::vec3 border_color;

private:
    const model::boards::Tile& _tile;

    static glm::vec3 GetColorForTileType( model::boards::TileType tile_type );
};
}
