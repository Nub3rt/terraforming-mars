#pragma once

#include <functional>

#include "player.h"
#include "tile_type.h"

namespace model::board
{
class Tile
{
public:
    Tile( const std::function<void( Player* )>& apply_placement_bonuses, TileType type = TileType::NONE ) noexcept;

    inline TileType get_type() const noexcept;
    inline void set_type( TileType type ) noexcept;
    inline Player* get_owner() const noexcept;
    inline void set_owner( Player* owner ) noexcept;
    
    void ApplyPlacementBonuses();

private:
    TileType _type;
    Player* _owner;
    const std::function<void( Player* )> _apply_placement_bonuses;
};
}
