#include "board.h"

#include <utility>
#include <vector>

namespace model::board
{
using pii = std::pair<int, int>;

Board::Board( ConcreteBoard* board ) noexcept : _concrete_board( board ) {}

Board::~Board() noexcept {
    delete _concrete_board;
}

inline const Tile& Board::operator()( int q, int r ) const noexcept {
    return _concrete_board->get_tile( q, r );
}

inline const Tile& Board::get_tile( int q, int r ) const noexcept {
    return _concrete_board->get_tile( q, r );
}

inline std::vector<pii> Board::GetNeighbouringTiles( int q, int r ) const {
    return _concrete_board->GetNeighbouringTiles( q, r );
}

inline std::vector<pii> Board::GetNeighbouringTilesOfType( int q, int r, TileType type ) const {
    return _concrete_board->GetNeighbouringTilesOfType( q, r, type );
}

inline std::vector<pii> Board::GetNeighbouringTilesOfTypeRange( int q, int r, TileType min, TileType max ) const {
    return _concrete_board->GetNeighbouringTilesOfTypeRange( q, r, min, max );
}

std::vector<pii> Board::GetTilesOfType( TileType type ) const {
    std::vector<pii> tiles;

    for ( auto it = begin(); it != end(); ++it ) {
        if ( it->get_type() == type )
            tiles.push_back( it.GetIndices() );
    }

    return tiles;
}

std::vector<pii> Board::GetPlaceableTilesOfType( const Player* player, TileType type ) const {
    std::vector<pii> tiles;

    for ( auto it = begin(); it != end(); ++it ) {
        if ( it->get_type() == type && (it->get_owner() == nullptr || it->get_owner() == player) )
            tiles.push_back( it.GetIndices() );
    }

    return tiles;
}

std::vector<pii> Board::GetEmptyTiles( const Player* player ) const {
    return GetPlaceableTilesOfType( player, TileType::EMPTY );
}

std::vector<pii> Board::GetValidOceanTiles( const Player* player ) const {
    return GetPlaceableTilesOfType( player, TileType::RESERVED_FOR_OCEAN );
}

std::vector<pii> Board::GetValidGreeneryTiles( const Player* player ) const {
    std::vector<pii> tiles_with_neighbour_of_owner;

    for ( auto it = begin(); it != end(); ++it ) {
        if ( it->get_type() == TileType::EMPTY && (it->get_owner() == nullptr || it->get_owner() == player) ) {
            auto [ q, r ] = it.GetIndices();
            std::vector<pii> neighbours = GetNeighbouringTiles( q, r );

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

std::vector<pii> Board::GetValidCityTiles( const Player* player ) const {
    std::vector<pii> tiles_with_no_city_neighbour;

    for ( auto it = begin(); it != end(); ++it ) {
        if ( it->get_type() == TileType::EMPTY && (it->get_owner() == nullptr || it->get_owner() == player) ) {
            auto [q, r] = it.GetIndices();
            std::vector<pii> neighbours = GetNeighbouringTiles( q, r );

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

std::vector<pii> Board::GetValidNoctisCityTiles( const Player* player ) const {
    const pii* ptr_to_index = _concrete_board->NoctisCityIndex();

    if ( ptr_to_index )
        return { pii( ptr_to_index->first, ptr_to_index->second ) };

    return GetValidCityTiles( player );
}

std::vector<pii> Board::GetValidLonelyCityTiles( const Player* player ) const {
    std::vector<pii> tiles_with_no_neighbours;

    for ( auto it = begin(); it != end(); ++it ) {
        if ( it->get_type() == TileType::EMPTY && (it->get_owner() == nullptr || it->get_owner() == player) ) {
            auto [q, r] = it.GetIndices();
            std::vector<pii> neighbours = GetNeighbouringTiles( q, r );

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

std::vector<pii> Board::GetValidUrbanizedAreaTiles( const Player* player ) const {
    std::vector<pii> tiles_with_min_two_city_neighbours;

    for ( auto it = begin(); it != end(); ++it ) {
        if ( it->get_type() == TileType::EMPTY && (it->get_owner() == nullptr || it->get_owner() == player) ) {
            auto [q, r] = it.GetIndices();
            std::vector<pii> neighbours = GetNeighbouringTiles( q, r );

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

inline void Board::PlaceTile( int q, int r, Player* player, TileType type ) {
    _concrete_board->PlaceTile( q, r, player, type );
}

inline void Board::SetOwner( int q, int r, Player* player ) {
    _concrete_board->SetOwner( q, r, player );
}

inline void Board::SetTileType( int q, int r, TileType type ) {
    _concrete_board->SetTileType( q, r, type );
}

inline void Board::SetOnTilePlacedCallback( std::function<void( int, int, const Tile& )> callback ) {
    _concrete_board->SetOnTilePlacedCallback( callback );
}

inline ConcreteBoard::IteratorWrapper Board::begin() const {
    return _concrete_board->begin();
}

inline ConcreteBoard::IteratorWrapper Board::end() const {
    return _concrete_board->end();
}
}
