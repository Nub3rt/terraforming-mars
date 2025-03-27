#include "board.h"

#include <utility>
#include <vector>

namespace model::boards
{
Board::Board( ConcreteBoard* board ) noexcept : _concrete_board( board ) {}

Board::~Board() noexcept {
    delete _concrete_board;
}

std::vector<std::pair<int, int>> Board::GetTilesOfType( TileType type ) const {
    std::vector<std::pair<int, int>> tiles;

    for ( auto it = begin(); it != end(); ++it ) {
        if ( it->get_type() == type )
            tiles.push_back( it->get_indices() );
    }

    return tiles;
}

std::vector<std::pair<int, int>> Board::GetPlaceableTilesOfType( const Player* player, TileType type ) const {
    std::vector<std::pair<int, int>> tiles;

    for ( auto it = begin(); it != end(); ++it ) {
        if ( it->get_type() == type && (it->get_owner() == nullptr || it->get_owner() == player) )
            tiles.push_back( it->get_indices() );
    }

    return tiles;
}

std::vector<std::pair<int, int>> Board::GetEmptyTiles( const Player* player ) const {
    return GetPlaceableTilesOfType( player, TileType::EMPTY );
}

std::vector<std::pair<int, int>> Board::GetValidOceanTiles( const Player* player ) const {
    return GetPlaceableTilesOfType( player, TileType::RESERVED_FOR_OCEAN );
}

std::vector<std::pair<int, int>> Board::GetValidGreeneryTiles( const Player* player ) const {
    std::vector<std::pair<int, int>> tiles_with_neighbour_of_owner;

    for ( auto it = begin(); it != end(); ++it ) {
        if ( it->get_type() == TileType::EMPTY && (it->get_owner() == nullptr || it->get_owner() == player) ) {
            auto [ q, r ] = it->get_indices();
            std::vector<std::pair<int, int>> neighbours = GetNeighbouringTiles( q, r );

            for ( const auto& [ nq, nr ] : neighbours ) {
                if ( get_tile( nq, nr ).get_owner() == player ) {
                    tiles_with_neighbour_of_owner.emplace_back( q, r );
                    break;
                }
            }
        }
    }

    if ( tiles_with_neighbour_of_owner.size() > 0 )
        return tiles_with_neighbour_of_owner;

    return GetEmptyTiles( player );
}

std::vector<std::pair<int, int>> Board::GetValidCityTiles( const Player* player ) const {
    std::vector<std::pair<int, int>> tiles_with_no_city_neighbour;

    for ( auto it = begin(); it != end(); ++it ) {
        if ( it->get_type() == TileType::EMPTY && (it->get_owner() == nullptr || it->get_owner() == player) ) {
            auto [q, r] = it->get_indices();
            std::vector<std::pair<int, int>> neighbours = GetNeighbouringTiles( q, r );

            bool no_city_neighbour = true;
            for ( const auto& [nq, nr] : neighbours ) {
                if ( get_tile( nq, nr ).get_type() == TileType::CITY ) {
                    no_city_neighbour = false;
                    break;
                }
            }

            if ( no_city_neighbour )
                tiles_with_no_city_neighbour.emplace_back( q, r );
        }
    }

    return tiles_with_no_city_neighbour;
}

std::vector<std::pair<int, int>> Board::GetValidNoctisCityTiles( const Player* player ) const {
    const std::pair<int, int>* ptr_to_index = _concrete_board->NoctisCityIndex();

    if ( ptr_to_index )
        return { std::pair<int, int>( ptr_to_index->first, ptr_to_index->second ) };

    return GetValidCityTiles( player );
}

std::vector<std::pair<int, int>> Board::GetValidLonelyCityTiles( const Player* player ) const {
    std::vector<std::pair<int, int>> tiles_with_no_neighbours;

    for ( auto it = begin(); it != end(); ++it ) {
        if ( it->get_type() == TileType::EMPTY && (it->get_owner() == nullptr || it->get_owner() == player) ) {
            auto [q, r] = it->get_indices();
            std::vector<std::pair<int, int>> neighbours = GetNeighbouringTiles( q, r );

            bool no_neighbours = true;
            for ( const auto& [nq, nr] : neighbours ) {
                if ( get_tile( nq, nr ).get_type() > TileType::EMPTY_MAX ) {
                    no_neighbours = false;
                    break;
                }
            }

            if ( no_neighbours )
                tiles_with_no_neighbours.emplace_back( q, r );
        }
    }

    return tiles_with_no_neighbours;
}

std::vector<std::pair<int, int>> Board::GetValidUrbanizedAreaTiles( const Player* player ) const {
    std::vector<std::pair<int, int>> tiles_with_min_two_city_neighbours;

    for ( auto it = begin(); it != end(); ++it ) {
        if ( it->get_type() == TileType::EMPTY && (it->get_owner() == nullptr || it->get_owner() == player) ) {
            auto [q, r] = it->get_indices();
            std::vector<std::pair<int, int>> neighbours = GetNeighbouringTiles( q, r );

            int city_neighbour_count = 0;
            for ( const auto& [nq, nr] : neighbours ) {
                if ( get_tile( nq, nr ).get_type() == TileType::CITY ) {
                    ++city_neighbour_count;
                }
            }

            if ( city_neighbour_count >= 2 )
                tiles_with_min_two_city_neighbours.emplace_back( q, r );
        }
    }

    return tiles_with_min_two_city_neighbours;
}
}
