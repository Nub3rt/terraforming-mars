#pragma once

#include "animatable.hpp"

#include "gl_utils/gl_utils.hpp"

#include "../model/boards/tile.hpp"
#include "../model/boards/tile_type.hpp"

namespace view
{
class TileWrapper
{
public:
    enum class MeshType
    {
        PLAINS,
        DUNES,
        MOUNTAINS,
    };
    using Special = model::boards::TileType;

    TileWrapper( const model::boards::Tile& tile );
    ~TileWrapper();

    inline const model::boards::Tile* operator->() noexcept { return &_tile; }
    inline const model::boards::Tile& operator*() noexcept { return _tile; }

    void OnTilePlaced();
    void Update( float delta );

    bool selectable = false;
    bool show_resources;
    glm::vec3 border_color;

    MeshType mesh = MeshType::DUNES;
    Special special = Special::NONE;
    Animatable<float> mars_to_special = 0.0f;
    Animatable<float> special_z;
    glm::mat4 pos_translate = {};
    glm::vec4 tex_ranges = {}; // start_u, end_u, start_v, end_v

private:
    const model::boards::Tile& _tile;
};
}
