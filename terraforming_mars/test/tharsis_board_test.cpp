#include "pch.h"

#include <string>

#include "../model/boards/tile.h"
#include "../model/boards/tile_type.h"

using namespace model;
using namespace boards;

namespace test
{
class TharsisBoardTest : public testing::Test
{
protected:
    TharsisConcreteBoard tharsis_board;
    Player player;
    std::string wrong_event_called = "";

    TharsisBoardTest() : tharsis_board(), player( 20, 0 ) {
        SetAllCallback( [ this ]( std::string event_name ) { wrong_event_called = event_name; } );
    }

    void SetAllCallback( std::function<void( std::string )> callback ) {
        player.SetOnDrawCardCallback( [ callback ]( Player* ) { callback( "OnDrawCard" ); } );
        player.SetOnRaiseTRCallback( [ callback ]( Player*, int ) { callback( "OnRaiseTR" ); } );
        player.SetOnRaiseTemperatureCallback( [ callback ]( Player* ) { callback( "OnRaiseTemperature" ); } );
        player.SetOnPlaceOceanCallback( [ callback ]( Player* ) { callback( "OnPlaceOcean" ); } );
        player.SetOnPlaceOceanOnNonOceanCallback( [ callback ]( Player* ) { callback( "OnPlaceOceanOnNonOcean" ); } );
        player.SetOnRaiseOxygenCallback( [ callback ]( Player* ) { callback( "OnRaiseOxygen" ); } );
        player.SetOnPlaceGreeneryCallback( [ callback ]( Player* ) { callback( "OnPlaceGreenery" ); } );
        player.SetOnPlaceGreeneryOnOceanCallback( [ callback ]( Player* ) { callback( "OnPlaceGreeneryOnOcean" ); } );
        player.SetOnPlaceCityCallback( [ callback ]( Player* ) { callback( "OnPlaceCity" ); } );
        player.SetOnPlaceNoctisCityCallback( [ callback ]( Player* ) { callback( "OnPlaceNoctisCity" ); } );
        player.SetOnPlaceLonelyCityCallback( [ callback ]( Player* ) { callback( "OnPlaceLonelyCity" ); } );
        player.SetOnPlaceUrbanizedAreaCallback( [ callback ]( Player* ) { callback( "OnPlaceUrbanizedArea" ); } );
        player.SetOnResourceAmountChangedCallback( [ callback ]( Player*, Resource, int ) { callback( "OnResourceAmountChanged" ); } );
        player.SetOnResourceProductionAmountChangedCallback( [ callback ]( Player*, Resource, int ) { callback( "OnResourceProductionAmountChanged" ); } );
        player.SetOnDestroyResourceCallback( [ callback ]( Player*, Resource, int ) { callback( "OnDestroyResource" ); } );
        player.SetOnDestroyResourceProductionCallback( [ callback ]( Player*, Resource, int ) { callback( "OnDestroyResourceProduction" ); } );
        player.SetOnConfirmSteelPaymentCallback( [ callback ]( Player*, int, std::function<void()> ) { callback( "OnConfirmSteelPayment" ); } );
        player.SetOnConfirmTitaniumPaymentCallback( [ callback ]( Player*, int, std::function<void()> ) { callback( "OnConfirmTitaniumPayment" ); } );
    }
};

TEST_F( TharsisBoardTest, GetSetTest ) {
    Tile& tile_1 = tharsis_board.get_tile( 4, 0 );
    EXPECT_EQ( nullptr, tile_1.get_owner() );
    EXPECT_EQ( TileType::EMPTY, tile_1.get_type() );

    Tile& tile_2 = tharsis_board.get_tile( 5, 4 );
    EXPECT_EQ( nullptr, tile_2.get_owner() );
    EXPECT_EQ( TileType::RESERVED_FOR_OCEAN, tile_2.get_type() );

    tharsis_board.SetOwner( 4, 0, &player );
    EXPECT_EQ( &player, tile_1.get_owner() );
    EXPECT_EQ( TileType::EMPTY, tile_1.get_type() );

    Tile& tile_1_2 = tharsis_board.get_tile( 4, 0 );
    EXPECT_EQ( &player, tile_1_2.get_owner() );
    EXPECT_EQ( TileType::EMPTY, tile_1_2.get_type() );

    tharsis_board.SetTileType( 5, 4, TileType::GREENERY );
    EXPECT_EQ( nullptr, tile_2.get_owner() );
    EXPECT_EQ( TileType::GREENERY, tile_2.get_type() );
}

TEST_F( TharsisBoardTest, PlaceTileTest ) {
    int player_call_count = 0;
    player.SetOnResourceAmountChangedCallback( [ this, &player_call_count ]( Player* p, Resource resource, int amount ) {
        EXPECT_EQ( &player, p );
        EXPECT_EQ( Resource::STEEL, resource );
        EXPECT_EQ( 2, amount );
        ++player_call_count;
    } );

    int board_call_count = 0;
    tharsis_board.SetOnTilePlacedCallback( [ this, &player_call_count ]( int q, int r, const Tile& tile ) {
        EXPECT_EQ( 4, q );
        EXPECT_EQ( 0, r );
    } );

    tharsis_board.PlaceTile( 4, 0, &player, TileType::GREENERY );
    EXPECT_EQ( 1, player_call_count );
    EXPECT_EQ( "", wrong_event_called );

    EXPECT_THROW( { tharsis_board.PlaceTile( 4, 0, &player, TileType::OCEAN ); }, std::logic_error );
    EXPECT_EQ( 1, player_call_count );
    EXPECT_EQ( "", wrong_event_called );
}

TEST_F( TharsisBoardTest, NoctisCityTest ) {
    const std::pair<int, int>* noctis_position = tharsis_board.NoctisCityIndex();
    ASSERT_NE( nullptr, noctis_position );

    auto [q, r] = *noctis_position;
    EXPECT_EQ( 2, q );
    EXPECT_EQ( 4, r );

    for ( auto it = tharsis_board.begin(); it != tharsis_board.end(); ++it ) {
        if ( it->get_indices() == *noctis_position )
            EXPECT_EQ( TileType::RESERVED_FOR_NOCTIS, it->get_type() );
        else
            EXPECT_NE( TileType::RESERVED_FOR_NOCTIS, it->get_type() );
    }
}

TEST_F( TharsisBoardTest, TopLeftGetNeighboursTest ) {
    std::set<std::pair<int, int>> expected_ns = { { 5, 0 }, { 4, 1 }, { 3, 1 } };
    std::vector<std::pair<int, int>> neighbours = tharsis_board.GetNeighbouringTiles( 4, 0 );
    std::set<std::pair<int, int>> actual_ns( neighbours.begin(), neighbours.end() );
    ASSERT_EQ( neighbours.size(), actual_ns.size() );

    EXPECT_EQ( expected_ns, actual_ns );
}

TEST_F( TharsisBoardTest, TopRightGetNeighboursTest ) {
    std::set<std::pair<int, int>> expected_ns = { { 7, 0 }, { 7, 1 }, { 8, 1 } };
    std::vector<std::pair<int, int>> neighbours = tharsis_board.GetNeighbouringTiles( 8, 0 );
    std::set<std::pair<int, int>> actual_ns( neighbours.begin(), neighbours.end() );
    ASSERT_EQ( neighbours.size(), actual_ns.size() );

    EXPECT_EQ( expected_ns, actual_ns );
}

TEST_F( TharsisBoardTest, RightGetNeighboursTest ) {
    std::set<std::pair<int, int>> expected_ns = { { 8, 3 }, { 7, 4 }, { 7, 5 } };
    std::vector<std::pair<int, int>> neighbours = tharsis_board.GetNeighbouringTiles( 8, 4 );
    std::set<std::pair<int, int>> actual_ns( neighbours.begin(), neighbours.end() );
    ASSERT_EQ( neighbours.size(), actual_ns.size() );

    EXPECT_EQ( expected_ns, actual_ns );
}

TEST_F( TharsisBoardTest, BottomRightGetNeighboursTest ) {
    std::set<std::pair<int, int>> expected_ns = { { 5, 7 }, { 4, 7 }, { 3, 8 } };
    std::vector<std::pair<int, int>> neighbours = tharsis_board.GetNeighbouringTiles( 4, 8 );
    std::set<std::pair<int, int>> actual_ns( neighbours.begin(), neighbours.end() );
    ASSERT_EQ( neighbours.size(), actual_ns.size() );

    EXPECT_EQ( expected_ns, actual_ns );
}

TEST_F( TharsisBoardTest, BottomLeftGetNeighboursTest ) {
    std::set<std::pair<int, int>> expected_ns = { { 0, 7 }, { 1, 7 }, { 1, 8 } };
    std::vector<std::pair<int, int>> neighbours = tharsis_board.GetNeighbouringTiles( 0, 8 );
    std::set<std::pair<int, int>> actual_ns( neighbours.begin(), neighbours.end() );
    ASSERT_EQ( neighbours.size(), actual_ns.size() );

    EXPECT_EQ( expected_ns, actual_ns );
}

TEST_F( TharsisBoardTest, LeftGetNeighboursTest ) {
    std::set<std::pair<int, int>> expected_ns = { { 1, 3 }, { 1, 4 }, { 0, 5 } };
    std::vector<std::pair<int, int>> neighbours = tharsis_board.GetNeighbouringTiles( 0, 4 );
    std::set<std::pair<int, int>> actual_ns( neighbours.begin(), neighbours.end() );
    ASSERT_EQ( neighbours.size(), actual_ns.size() );

    EXPECT_EQ( expected_ns, actual_ns );
}

TEST_F( TharsisBoardTest, MiddleGetNeighboursTest ) {
    std::set<std::pair<int, int>> expected_ns = { { 3, 5 }, { 4, 5 }, { 4, 6 }, { 3, 7 }, { 2, 7 }, { 2, 6 } };
    std::vector<std::pair<int, int>> neighbours = tharsis_board.GetNeighbouringTiles( 3, 6 );
    std::set<std::pair<int, int>> actual_ns( neighbours.begin(), neighbours.end() );
    ASSERT_EQ( neighbours.size(), actual_ns.size() );

    EXPECT_EQ( expected_ns, actual_ns );
}

TEST_F( TharsisBoardTest, Test ) {
    std::set<std::pair<int, int>> expected_ns = { { 6, 3 }, { 7, 3 }, { 7, 4 } };
    std::vector<std::pair<int, int>> neighbours = tharsis_board.GetNeighbouringTilesOfType( 6, 4, TileType::EMPTY );
    std::set<std::pair<int, int>> actual_ns( neighbours.begin(), neighbours.end() );
    ASSERT_EQ( neighbours.size(), actual_ns.size() );

    EXPECT_EQ( expected_ns, actual_ns );

    expected_ns = { { 6, 5 }, { 5, 5 }, { 5, 4 } };
    neighbours = tharsis_board.GetNeighbouringTilesOfType( 6, 4, TileType::RESERVED_FOR_OCEAN );
    actual_ns = std::set<std::pair<int, int>>( neighbours.begin(), neighbours.end() );
    ASSERT_EQ( neighbours.size(), actual_ns.size() );

    EXPECT_EQ( expected_ns, actual_ns );
}

TEST_F( TharsisBoardTest, IteratorTest ) {
    for ( auto it = tharsis_board.begin(); it != tharsis_board.end(); ++it )
        EXPECT_NE( TileType::NONE, it->get_type() );
}
}
