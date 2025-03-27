#include "pch.h"

#include "../model/boards/tile.h"
#include "../model/boards/tile_type.h"

using namespace model;
using namespace boards;

namespace test
{
std::function<void( Player* )> null = []( Player* player ) {};

TEST( TileTest, ConstructorTest ) {
    Tile tile1( 4, 4, null );
    EXPECT_EQ( 4, tile1.q );
    EXPECT_EQ( 4, tile1.r );
    std::pair<int, int> p1( 4, 4 );
    EXPECT_EQ( p1, tile1.get_indices() );
    EXPECT_EQ( nullptr, tile1.get_owner() );
    EXPECT_EQ( TileType::NONE, tile1.get_type() );

    Tile tile2( 3, 7, null, TileType::RESERVED_FOR_OCEAN );
    std::pair<int, int> p2( 3, 7 );
    EXPECT_EQ( p2, tile2.get_indices() );
    EXPECT_EQ( 3, tile2.q );
    EXPECT_EQ( 7, tile2.r );
    EXPECT_EQ( nullptr, tile2.get_owner() );
    EXPECT_EQ( TileType::RESERVED_FOR_OCEAN, tile2.get_type() );
}

TEST( TileTest, GettersSettersTest ) {
    Tile tile( 4, 4, null, TileType::EMPTY );
    Player player( 20, 0 );

    EXPECT_EQ( nullptr, tile.get_owner() );
    EXPECT_EQ( TileType::EMPTY, tile.get_type() );

    tile.set_owner( &player );
    EXPECT_EQ( &player, tile.get_owner() );
    EXPECT_EQ( TileType::EMPTY, tile.get_type() );

    tile.set_type( TileType::GREENERY );
    EXPECT_EQ( &player, tile.get_owner() );
    EXPECT_EQ( TileType::GREENERY, tile.get_type() );

    tile.set_owner( nullptr );
    EXPECT_EQ( nullptr, tile.get_owner() );
    EXPECT_EQ( TileType::GREENERY, tile.get_type() );

    tile.set_type( TileType::OCEAN );
    EXPECT_EQ( nullptr, tile.get_owner() );
    EXPECT_EQ( TileType::OCEAN, tile.get_type() );
}

TEST( TileTest, ApplyPlacementBonusTest ) {
    bool flag = false;
    std::function<void( Player* )> callback = [ &flag ]( Player* player ) { flag = true; };
    Tile tile( 4, 4, callback );
    Player player( 20, 0 );

    EXPECT_THROW( { tile.ApplyPlacementBonuses(); }, std::logic_error );

    EXPECT_EQ( false, flag );

    tile.set_owner( &player );
    tile.ApplyPlacementBonuses();

    EXPECT_EQ( true, flag );
}
}
