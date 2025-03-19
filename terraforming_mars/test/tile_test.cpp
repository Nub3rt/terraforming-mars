#include "pch.h"


#include "../model/boards/tile.h"
#include "../model/boards/tile_type.h"

std::function<void( model::Player* )> noop = []( model::Player* player ) {};

TEST( TileTest, ConstructorTest ) {
    model::boards::Tile tile1( noop );
    EXPECT_EQ( nullptr, tile1.get_owner() );
    EXPECT_EQ( model::boards::TileType::NONE, tile1.get_type() );

    model::boards::Tile tile2( noop, model::boards::TileType::RESERVED_FOR_OCEAN );
    EXPECT_EQ( nullptr, tile2.get_owner() );
    EXPECT_EQ( model::boards::TileType::RESERVED_FOR_OCEAN, tile2.get_type() );
}

TEST( TileTest, GettersSettersTest ) {
    model::boards::Tile tile( noop, model::boards::TileType::EMPTY );
    model::Player player( 20, 0 );

    EXPECT_EQ( nullptr, tile.get_owner() );
    EXPECT_EQ( model::boards::TileType::EMPTY, tile.get_type() );

    tile.set_owner( &player );
    EXPECT_EQ( &player, tile.get_owner() );
    EXPECT_EQ( model::boards::TileType::EMPTY, tile.get_type() );

    tile.set_type( model::boards::TileType::GREENERY );
    EXPECT_EQ( &player, tile.get_owner() );
    EXPECT_EQ( model::boards::TileType::GREENERY, tile.get_type() );

    tile.set_owner( nullptr );
    EXPECT_EQ( nullptr, tile.get_owner() );
    EXPECT_EQ( model::boards::TileType::GREENERY, tile.get_type() );

    tile.set_type( model::boards::TileType::OCEAN );
    EXPECT_EQ( nullptr, tile.get_owner() );
    EXPECT_EQ( model::boards::TileType::OCEAN, tile.get_type() );
}

TEST( TileTest, ApplyPlacementBonusTest ) {
    bool flag = false;
    std::function<void( model::Player* )> callback = [ &flag ]( model::Player* player ) { flag = true; };
    model::boards::Tile tile( callback );
    model::Player player( 20, 0 );

    EXPECT_THROW( { tile.ApplyPlacementBonuses(); }, std::logic_error );

    EXPECT_EQ( false, flag );

    tile.set_owner( &player );
    tile.ApplyPlacementBonuses();

    EXPECT_EQ( true, flag );
}