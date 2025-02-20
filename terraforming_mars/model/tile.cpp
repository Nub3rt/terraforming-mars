#include "tile.h"

#include <functional>
#include <stdexcept>

#include "player.h"

namespace model::board
{
Tile::Tile( const std::function<void( Player* )>& apply_placement_bonuses, TileType type ) noexcept
    : _type( type ), _owner( nullptr ), _apply_placement_bonuses( apply_placement_bonuses ) {}

inline TileType Tile::get_type() const noexcept { return _type; }
inline void Tile::set_type( TileType type ) noexcept { _type = type; }
inline Player* Tile::get_owner() const noexcept { return _owner; }
inline void Tile::set_owner( Player* owner ) noexcept { _owner = owner; }

void Tile::ApplyPlacementBonuses() {
    if ( _owner == nullptr )
        throw std::logic_error( "Tile: cannot apply placement bonuses without owner" );

    _apply_placement_bonuses( _owner );
}
}
