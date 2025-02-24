#include "concrete_board.h"

#include <iostream>
#include <vector>
#include <functional>
#include <stdexcept>

#include "player.h"
#include "resource.h"
#include "tile_type.h"

namespace model::board
{
bool ConcreteBoard::CanPlaceTile( int q, int r, const Player* player, TileType type ) const {
    const Tile& tile = this->operator()( q, r );

    if ( tile.get_type() == TileType::NONE ) {
        std::cerr << "ConcreteBoard was indexed on an invalid tile\n";
        return false;
    }

    if ( tile.get_owner() != nullptr && tile.get_owner() != player )
        return false;

    if ( tile.get_type() == TileType::RESERVED_FOR_OCEAN ) {
        if ( type == TileType::OCEAN )
            return true;
        else
            return false;
    }

    if ( tile.get_type() != TileType::EMPTY )
        return false;

    if ( type == TileType::CITY ) {
        if ( NeighbouringTilesOfType( q, r, TileType::CITY ) > 0 )
            return false;
    }

    return true;
}

void ConcreteBoard::PlaceTile( int q, int r, Player* player, TileType type ) {
    if ( !CanPlaceTile( q, r, player, type ) )
        throw std::logic_error( "ConcreteBoard::PlaceTile: PlaceTile called while tile cannot be placed!" );

    Tile& tile = get_tile( q, r );
    tile.set_type( type );
    tile.set_owner( player );
    tile.ApplyPlacementBonuses();
}

int ConcreteBoard::NeighbouringTiles( int q, int r ) const {
    std::vector<std::reference_wrapper<const Tile>> neighbours = GetNeighbours( q, r );

    int count = 0;
    for ( const Tile& neighbour : neighbours ) {
        if ( neighbour.get_type() >= TileType::EMPTY_MIN && neighbour.get_type() <= TileType::EMPTY_MAX )
            ++count;
    }

    return count;
}

int ConcreteBoard::NeighbouringTilesOfType( int q, int r, TileType type ) const {
    std::vector<std::reference_wrapper<const Tile>> neighbours = GetNeighbours( q, r );

    int count = 0;
    for ( const Tile& neighbour : neighbours ) {
        if ( neighbour.get_type() == type )
            ++count;
    }

    return count;
}

ConcreteBoard::ConcreteBoard() noexcept {}
ConcreteBoard::~ConcreteBoard() noexcept {}


const std::function<void( Player* )> ConcreteBoard::_noop              = []( Player* player ) {};
const std::function<void( Player* )> ConcreteBoard::_draw_one_card     = []( Player* player ) { player->DrawCard(); };
const std::function<void( Player* )> ConcreteBoard::_draw_two_cards    = []( Player* player ) { player->DrawCard(); player->DrawCard(); };
const std::function<void( Player* )> ConcreteBoard::_gain_one_steel    = []( Player* player ) { player->GainResource( Resource::STEEL,    1 ); };
const std::function<void( Player* )> ConcreteBoard::_gain_two_steel    = []( Player* player ) { player->GainResource( Resource::STEEL,    2 ); };
const std::function<void( Player* )> ConcreteBoard::_gain_one_titanium = []( Player* player ) { player->GainResource( Resource::TITANIUM, 1 ); };
const std::function<void( Player* )> ConcreteBoard::_gain_two_titanium = []( Player* player ) { player->GainResource( Resource::TITANIUM, 2 ); };
const std::function<void( Player* )> ConcreteBoard::_gain_one_plants   = []( Player* player ) { player->GainResource( Resource::PLANTS,   1 ); };
const std::function<void( Player* )> ConcreteBoard::_gain_two_plants   = []( Player* player ) { player->GainResource( Resource::PLANTS,   2 ); };

const std::function<void( Player* )> ConcreteBoard::_gain_titanium_and_plants =
    []( Player* player ) { player->GainResource( Resource::TITANIUM, 1 ); player->GainResource( Resource::PLANTS, 1 ); };

const std::function<void( Player* )> ConcreteBoard::_warn_bad_index = []( Player* player ) { std::cerr << "ConcreteBoard was indexed on an invalid tile\n"; };
}
