#include "tile.h"

#include <functional>
#include <stdexcept>

#include "../player.h"

namespace model::boards
{
Tile::Tile( int q, int r, const std::function<void( Player* )>& apply_placement_bonuses, TileType type ) noexcept
    : q( q ), r( r ), _type( type ), _owner( nullptr ), _apply_placement_bonuses( apply_placement_bonuses ) {}

void Tile::ApplyPlacementBonuses() {
    if ( _owner == nullptr )
        throw std::logic_error( "Tile::ApplyPlacementBonuses: cannot apply placement bonuses without owner!" );

    _apply_placement_bonuses( _owner );
}
}
