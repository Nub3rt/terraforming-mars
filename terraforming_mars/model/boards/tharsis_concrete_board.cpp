#include "tharsis_concrete_board.hpp"

#include <array>
#include <functional>
#include <stdexcept>

#include "concrete_board.hpp"
#include "hexagonal_grid.hpp"
#include "tile.hpp"
#include "../constants.hpp"
#include "../player.hpp"

namespace model::boards
{
TharsisConcreteBoard::TharsisConcreteBoard() noexcept : _board( get_starting_board() ) {}

TharsisConcreteBoard::~TharsisConcreteBoard() noexcept {}

const Tile& TharsisConcreteBoard::get_tile( int q, int r ) const noexcept { return _board[ r ][ q ]; }

std::vector<std::pair<int, int>> TharsisConcreteBoard::GetNeighbouringTiles( int q, int r ) const {
    std::vector<std::pair<int, int>> neighbours;

    int s = -q - r;
    if ( r > 0 && q < 8 )
        neighbours.emplace_back( q + 1, r - 1 );
    if ( q < 8 && s > -12 )
        neighbours.emplace_back( q + 1, r     );
    if ( s > -12 && r < 8 )
        neighbours.emplace_back( q    , r + 1 );
    if ( r < 8 && q > 0 )
        neighbours.emplace_back( q - 1, r + 1 );
    if ( q > 0 && s < -4 )
        neighbours.emplace_back( q - 1, r     );
    if ( s < -4 && r > 0 )
        neighbours.emplace_back( q    , r - 1 );

    return neighbours;
}

const std::pair<int, int>* TharsisConcreteBoard::NoctisCityIndex() const {
    return &_noctis_city_index;
}

void TharsisConcreteBoard::PlaceTile( int q, int r, Player* player, TileType type ) {
    Tile& tile = get_tile( q, r );

    if ( tile.get_type() == TileType::NONE )
        throw std::logic_error( "TharsisConcreteBoard::PlaceTile: tried to access invalid tyle (TileType::NONE)!" );

    if ( +tile.get_type() < +TileType::EMPTY_MIN || +tile.get_type() > +TileType::EMPTY_MAX )
        throw std::logic_error( "TharsisConcreteBoard::PlaceTile: tile was already placed here!" );

    if ( tile.get_owner() != nullptr && tile.get_owner() != player )
        throw std::logic_error( "TharsisConcreteBoard::PlaceTile: tile has another owner!" );

    tile.set_owner( player );
    tile.set_type( type );
    _on_tile_placed.Invoke( q, r, tile );
    tile.ApplyPlacementBonuses();
}

void TharsisConcreteBoard::SetOwner( int q, int r, Player* player ) {
    Tile& tile = get_tile( q, r );

    if ( tile.get_owner() != nullptr && tile.get_owner() != player )
        throw std::logic_error( "TharsisConcreteBoard::SetOwner: tile has another owner!" );

    tile.set_owner( player );
}

void TharsisConcreteBoard::SetTileType( int q, int r, TileType type ) {
    get_tile( q, r ).set_type( type );
}

constexpr std::pair<int, int> TharsisConcreteBoard::_noctis_city_index = std::pair<int, int>( 2, 4 );

const std::array<std::array<Tile, 9>, 9>& TharsisConcreteBoard::get_starting_board() {
    static const std::array<std::array<Tile, 9>, 9> starting_board = {{
        {{
            Tile( 0, 0, get_bad_index ()),
            Tile( 1, 0, get_bad_index ()),
            Tile( 2, 0, get_bad_index ()),
            Tile( 3, 0, get_bad_index ()),
            Tile( 4, 0, get_gain_two_steel(), TileType::EMPTY ),
            Tile( 5, 0, get_gain_two_steel(), TileType::RESERVED_FOR_OCEAN ),
            Tile( 6, 0, get_noop(), TileType::EMPTY ),
            Tile( 7, 0, get_draw_one_card(), TileType::RESERVED_FOR_OCEAN ),
            Tile( 8, 0, get_noop(), TileType::RESERVED_FOR_OCEAN ),
        }}, // first row
        {{
            Tile( 0, 1, get_bad_index() ),
            Tile( 1, 1, get_bad_index() ),
            Tile( 2, 1, get_bad_index() ),
            Tile( 3, 1, get_noop(), TileType::EMPTY ),
            Tile( 4, 1, get_gain_one_steel(), TileType::EMPTY ),
            Tile( 5, 1, get_noop(), TileType::EMPTY ),
            Tile( 6, 1, get_noop(), TileType::EMPTY ),
            Tile( 7, 1, get_noop(), TileType::EMPTY ),
            Tile( 8, 1, get_draw_two_cards(), TileType::RESERVED_FOR_OCEAN ),
        }}, // second row
        {{
            Tile( 0, 2, get_bad_index() ),
            Tile( 1, 2, get_bad_index() ),
            Tile( 2, 2, get_draw_one_card(), TileType::EMPTY ),
            Tile( 3, 2, get_noop(), TileType::EMPTY ),
            Tile( 4, 2, get_noop(), TileType::EMPTY ),
            Tile( 5, 2, get_noop(), TileType::EMPTY ),
            Tile( 6, 2, get_noop(), TileType::EMPTY ),
            Tile( 7, 2, get_noop(), TileType::EMPTY ),
            Tile( 8, 2, get_gain_one_steel(), TileType::EMPTY ),
        }}, // third row
        {{
            Tile( 0, 3, get_bad_index() ),
            Tile( 1, 3, get_gain_titanium_and_plants(), TileType::EMPTY ),
            Tile( 2, 3, get_gain_one_plants(), TileType::EMPTY ),
            Tile( 3, 3, get_gain_one_plants(), TileType::EMPTY ),
            Tile( 4, 3, get_gain_one_plants(), TileType::EMPTY ),
            Tile( 5, 3, get_gain_two_plants(), TileType::EMPTY ),
            Tile( 6, 3, get_gain_one_plants(), TileType::EMPTY ),
            Tile( 7, 3, get_gain_one_plants(), TileType::EMPTY ),
            Tile( 8, 3, get_gain_two_plants(), TileType::RESERVED_FOR_OCEAN ),
        }}, // fourth row
        {{
            Tile( 0, 4, get_gain_two_plants(), TileType::EMPTY ),
            Tile( 1, 4, get_gain_two_plants(), TileType::EMPTY ),
            Tile( 2, 4, get_gain_two_plants(), TileType::RESERVED_FOR_NOCTIS ),
            Tile( 3, 4, get_gain_two_plants(), TileType::RESERVED_FOR_OCEAN ),
            Tile( 4, 4, get_gain_two_plants(), TileType::RESERVED_FOR_OCEAN ),
            Tile( 5, 4, get_gain_two_plants(), TileType::RESERVED_FOR_OCEAN ),
            Tile( 6, 4, get_gain_two_plants(), TileType::EMPTY ),
            Tile( 7, 4, get_gain_two_plants(), TileType::EMPTY ),
            Tile( 8, 4, get_gain_two_plants(), TileType::EMPTY ),
        }}, // fifth row
        {{
            Tile( 0, 5, get_gain_one_plants(), TileType::EMPTY ),
            Tile( 1, 5, get_gain_two_plants(), TileType::EMPTY ),
            Tile( 2, 5, get_gain_one_plants(), TileType::EMPTY ),
            Tile( 3, 5, get_gain_one_plants(), TileType::EMPTY ),
            Tile( 4, 5, get_gain_one_plants(), TileType::EMPTY ),
            Tile( 5, 5, get_gain_one_plants(), TileType::RESERVED_FOR_OCEAN ),
            Tile( 6, 5, get_gain_one_plants(), TileType::RESERVED_FOR_OCEAN ),
            Tile( 7, 5, get_gain_one_plants(), TileType::RESERVED_FOR_OCEAN ),
            Tile( 8, 5, get_bad_index ()),
        }}, // sixth row
        {{
            Tile( 0, 6, get_noop(), TileType::EMPTY ),
            Tile( 1, 6, get_noop(), TileType::EMPTY ),
            Tile( 2, 6, get_noop(), TileType::EMPTY ),
            Tile( 3, 6, get_noop(), TileType::EMPTY ),
            Tile( 4, 6, get_noop(), TileType::EMPTY ),
            Tile( 5, 6, get_gain_one_plants(), TileType::EMPTY ),
            Tile( 6, 6, get_noop(), TileType::EMPTY ),
            Tile( 7, 6, get_bad_index() ),
            Tile( 8, 6, get_bad_index ()),
        }}, // seventh row
        {{
            Tile( 0, 7, get_gain_two_steel(), TileType::EMPTY ),
            Tile( 1, 7, get_noop(), TileType::EMPTY ),
            Tile( 2, 7, get_draw_one_card(), TileType::EMPTY ),
            Tile( 3, 7, get_draw_one_card(), TileType::EMPTY ),
            Tile( 4, 7, get_noop(), TileType::EMPTY ),
            Tile( 5, 7, get_gain_one_titanium(), TileType::EMPTY ),
            Tile( 6, 7, get_bad_index() ),
            Tile( 7, 7, get_bad_index() ),
            Tile( 8, 7, get_bad_index ()),
        }}, // eighth row
        {{
            Tile( 0, 8, get_gain_one_steel(), TileType::EMPTY ),
            Tile( 1, 8, get_gain_two_steel(), TileType::EMPTY ),
            Tile( 2, 8, get_noop(), TileType::EMPTY ),
            Tile( 3, 8, get_noop(), TileType::EMPTY ),
            Tile( 4, 8, get_gain_two_titanium(), TileType::RESERVED_FOR_OCEAN ),
            Tile( 5, 8, get_bad_index() ),
            Tile( 6, 8, get_bad_index() ),
            Tile( 7, 8, get_bad_index() ),
            Tile( 8, 8, get_bad_index ()),
        }}, // ninth row
    }};
    return starting_board;
}


TharsisConcreteBoard::TharsisIterator::TharsisIterator() :
    _ptr( nullptr ) {}

TharsisConcreteBoard::TharsisIterator::TharsisIterator( pointer ptr ) :
    _ptr( ptr ) {}

TharsisConcreteBoard::TharsisIterator::~TharsisIterator() {}

ConcreteBoard::Iterator::reference TharsisConcreteBoard::TharsisIterator::operator*() const {
    return *_ptr;
}

ConcreteBoard::Iterator::pointer TharsisConcreteBoard::TharsisIterator::operator->() const {
    return _ptr;
}

ConcreteBoard::Iterator& TharsisConcreteBoard::TharsisIterator::operator++() {
    if ( _ptr->r == 8 && _ptr->q == 5 )
        return *this;

    ++_ptr;

    if ( _ptr->get_type() == TileType::NONE )
        return ++(*this);

    return *this;
}

bool TharsisConcreteBoard::TharsisIterator::operator==( const Iterator& other ) const {
    const TharsisIterator* other_iterator = dynamic_cast<const TharsisIterator*>( &other );
    return other_iterator && _ptr == other_iterator->_ptr;
}

bool TharsisConcreteBoard::TharsisIterator::operator!=( const Iterator& other ) const {
    return !(*this == other);
}

ConcreteBoard::IteratorWrapper TharsisConcreteBoard::begin() const {
    return new TharsisIterator( &_board[ THARSIS_BEGIN_R ][ THARSIS_BEGIN_Q ] );
}

ConcreteBoard::IteratorWrapper TharsisConcreteBoard::end() const {
    return new TharsisIterator( &_board[ THARSIS_END_R ][ THARSIS_END_Q ] );
}
}
