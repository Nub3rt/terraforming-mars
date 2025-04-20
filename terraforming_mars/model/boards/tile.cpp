#include "tile.hpp"

#include <functional>
#include <stdexcept>

#include "../player.hpp"

namespace model::boards
{
Tile::Tile( int q, int r, const std::function<void( Player* )>& apply_placement_bonuses, TileType type ) noexcept
    : q( q ), r( r ), _type( type ), _owner( nullptr ), _apply_placement_bonuses( apply_placement_bonuses ) {}

Tile::Tile( int q, int r, const std::function<void( Player* )>& apply_placement_bonuses, TileType type, Resource bonus_1 ) noexcept
    : Tile( q, r, apply_placement_bonuses, type ) {
    _bonus_1 = bonus_1;
}

Tile::Tile( int q, int r, const std::function<void( Player* )>& apply_placement_bonuses, TileType type, Resource bonus_1, Resource bonus_2 ) noexcept
    : Tile( q, r, apply_placement_bonuses, type, bonus_1 ) {
    _bonus_2 = bonus_2;
}

bool Tile::IsEmpty() const {
    return _type >= TileType::EMPTY_MIN && _type <= TileType::EMPTY_MAX;
}

void Tile::ApplyPlacementBonuses( Player* player ) {
    if ( player == nullptr )
        throw std::logic_error( "Tile::ApplyPlacementBonuses: cannot apply placement bonuses without owner!" );

    _apply_placement_bonuses( player );
}
}
