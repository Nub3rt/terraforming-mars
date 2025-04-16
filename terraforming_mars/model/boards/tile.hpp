#pragma once

#include <functional>

#include "../player.hpp"
#include "tile_type.hpp"

namespace model::boards
{
class Tile
{
public:
    Tile( int q, int r, const std::function<void( Player* )>& apply_placement_bonuses, TileType type = TileType::NONE ) noexcept;

    inline TileType get_type() const noexcept { return _type; }
    inline void set_type( TileType type ) noexcept { _type = type; }
    inline Player* get_owner() const noexcept { return _owner; }
    inline void set_owner( Player* owner ) noexcept { _owner = owner; }
    inline std::pair<int, int> get_indices() const noexcept { return std::pair<int, int>( q, r ); }
    
    void ApplyPlacementBonuses();

    const int q, r;

private:
    TileType _type;
    Player* _owner;
    const std::function<void( Player* )> _apply_placement_bonuses;
};
}
