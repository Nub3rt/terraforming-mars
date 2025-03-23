#include "pch.h"

#include "gtest/gtest-typed-test.h"

#include <algorithm>

#include "../model/boards/tile.h"
#include "../model/boards/tile_type.h"

using namespace model;
using namespace model::boards;

namespace test
{
template<typename T>
class BoardTest : public testing::Test
{
public:
    BoardTest() : board( new T() ), player( 20, 0 ) {}

    Board board;
    Player player;
};

using ConcreteBoardTypes = testing::Types<TharsisConcreteBoard>;
TYPED_TEST_CASE( BoardTest, ConcreteBoardTypes );

TYPED_TEST( BoardTest, GetTest ) {
    const Tile& tile_1 = this->board( 4, 4 );
    const Tile& tile_2 = this->board.get_tile( 4, 4 );
    EXPECT_EQ( &tile_1, &tile_2 );
}

TYPED_TEST( BoardTest, TilesOfTypeTest ) {
    std::vector<std::pair<int, int>> tiles = this->board.GetTilesOfType( TileType::EMPTY );

    for ( auto it = this->board.begin(); it != this->board.end(); ++it ) {
        if ( std::find( tiles.cbegin(), tiles.cend(), it.GetIndices() ) != tiles.cend() )
            EXPECT_EQ( TileType::EMPTY, it->get_type() );
        else
            EXPECT_NE( TileType::EMPTY, it->get_type() );
    }
}

TYPED_TEST( BoardTest, ValidOceanTilesTest ) {
    std::vector<std::pair<int, int>> tiles = this->board.GetValidOceanTiles( &this->player );

    for ( auto& [q, r] : tiles )
        EXPECT_EQ( TileType::RESERVED_FOR_OCEAN, this->board( q, r ).get_type() );
}

TYPED_TEST( BoardTest, ValidGreeneryTilesTest ) {
    std::vector<std::pair<int, int>> tiles = this->board.GetValidGreeneryTiles( &this->player );

    for ( auto& [q, r] : tiles )
        EXPECT_EQ( TileType::EMPTY, this->board( q, r ).get_type() );
}

TYPED_TEST( BoardTest, ValidGreeneryTilesWithOwnedTileTest ) {
    auto it = this->board.begin();
    auto [q, r] = it.GetIndices();
    while ( it->get_type() != TileType::EMPTY && this->board.GetNeighbouringTilesOfType( q, r, TileType::EMPTY ).size() < 2 ) {
        ++it;
        std::tie( q, r ) = it.GetIndices();
    }

    this->board.PlaceTile( q, r, &this->player, TileType::CITY );
    TileType t_actual = this->board( q, r ).get_type();
    Player* p_actual = this->board( q, r ).get_owner();
    ASSERT_EQ( TileType::CITY, t_actual );
    ASSERT_EQ( &this->player, p_actual );

    std::vector<std::pair<int, int>> neighbours = this->board.GetNeighbouringTilesOfType( q, r, TileType::EMPTY );
    std::vector<std::pair<int, int>> valid_greenerys = this->board.GetValidGreeneryTiles( &this->player );

    std::set<std::pair<int, int>> expected( neighbours.begin(), neighbours.end() );
    std::set<std::pair<int, int>> actual( valid_greenerys.begin(), valid_greenerys.end() );

    EXPECT_EQ( expected, actual );
}

TYPED_TEST( BoardTest, ValidCityTilesTest ) {
    auto it = this->board.begin();
    auto [q, r] = it.GetIndices();
    while ( it->get_type() != TileType::EMPTY && this->board.GetNeighbouringTilesOfType( q, r, TileType::EMPTY ).size() < 2 ) {
        ++it;
        std::tie( q, r ) = it.GetIndices();
    }

    this->board.PlaceTile( q, r, &this->player, TileType::CITY );
    TileType t_actual = this->board( q, r ).get_type();
    Player* p_actual = this->board( q, r ).get_owner();
    ASSERT_EQ( TileType::CITY, t_actual );
    ASSERT_EQ( &this->player, p_actual );

    std::vector<std::pair<int, int>> neighbours = this->board.GetNeighbouringTilesOfType( q, r, TileType::EMPTY );
    std::vector<std::pair<int, int>> valid_cities = this->board.GetValidCityTiles( &this->player );

    for ( auto& indices : neighbours ) {
        auto find_it = std::find( valid_cities.cbegin(), valid_cities.cend(), indices );
        EXPECT_EQ( valid_cities.cend(), find_it );
    }
}

TYPED_TEST( BoardTest, ValidLonelyCityTilesTest ) {
    auto it = this->board.begin();
    auto [q, r] = it.GetIndices();
    while ( it->get_type() != TileType::EMPTY && this->board.GetNeighbouringTilesOfType( q, r, TileType::EMPTY ).size() < 2 ) {
        ++it;
        std::tie( q, r ) = it.GetIndices();
    }

    this->board.PlaceTile( q, r, &this->player, TileType::GREENERY );
    TileType t_actual = this->board( q, r ).get_type();
    Player* p_actual = this->board( q, r ).get_owner();
    ASSERT_EQ( TileType::GREENERY, t_actual );
    ASSERT_EQ( &this->player, p_actual );

    std::vector<std::pair<int, int>> neighbours = this->board.GetNeighbouringTilesOfType( q, r, TileType::EMPTY );
    std::vector<std::pair<int, int>> valid_lonely_cities = this->board.GetValidLonelyCityTiles( &this->player );

    for ( auto& indices : neighbours ) {
        auto find_it = std::find( valid_lonely_cities.cbegin(), valid_lonely_cities.cend(), indices );
        EXPECT_EQ( valid_lonely_cities.cend(), find_it );
    }
}

TYPED_TEST( BoardTest, ValidUrbanizedAreaTilesTest ) {
    auto it = this->board.begin();
    auto [q, r] = it.GetIndices();
    // this is not too safe
    while ( this->board( q, r     ).get_type() == TileType::EMPTY &&
            this->board( q, r + 1 ).get_type() == TileType::EMPTY &&
            this->board( q, r + 2 ).get_type() == TileType::EMPTY ) {
        ++it;
        std::tie( q, r ) = it.GetIndices();
    }

    this->board.PlaceTile( q, r    , &this->player, TileType::CITY );
    this->board.PlaceTile( q, r + 2, &this->player, TileType::CITY );

    std::pair<int, int> expected( q, r + 1 );
    std::vector<std::pair<int, int>> valid_lonely_cities = this->board.GetValidUrbanizedAreaTiles( &this->player );

    ASSERT_EQ( 1, valid_lonely_cities.size() );
    EXPECT_EQ( expected, valid_lonely_cities[0] );
}
}
