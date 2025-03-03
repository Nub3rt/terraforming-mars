#include "tharsis_concrete_board.h"

#include <array>
#include <functional>
#include <stdexcept>

#include "concrete_board.h"
#include "hexagonal_grid.h"
#include "player.h"
#include "tile.h"

namespace model::board
{
using pii = std::pair<int, int>;

TharsisConcreteBoard::TharsisConcreteBoard() noexcept : _board( _starting_board ) {}

TharsisConcreteBoard::~TharsisConcreteBoard() noexcept {}

inline Tile& TharsisConcreteBoard::get_tile( int q, int r ) noexcept {
    return _board[ r ][ q ];
}

std::vector<pii> TharsisConcreteBoard::GetNeighbouringTiles( int q, int r ) const {
    std::vector<pii> neighbours;

    int s = -q - r;
    if ( r > 0 && q < 8 )
        neighbours.emplace_back( q + 1, r - 1);
    if ( q < 8 && s > -12 )
        neighbours.emplace_back( q + 1, r    );
    if ( s > -12 && r < 8 )
        neighbours.emplace_back( q    , r + 1);
    if ( r < 8 && q > 0 )
        neighbours.emplace_back( q - 1, r + 1);
    if ( q > 0 && s < -4 )
        neighbours.emplace_back( q - 1, r    );
    if ( s < -4 && r > 0 )
        neighbours.emplace_back( q    , r - 1);

    return neighbours;
}

const pii* TharsisConcreteBoard::NoctisCityIndex() const {
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
    _on_tile_placed.Trigger( q, r, tile );
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

constexpr pii TharsisConcreteBoard::_noctis_city_index = pii( 2, 4 );

const std::array<std::array<Tile, 9>, 9> TharsisConcreteBoard::_starting_board = {{
    {{
        Tile( _bad_index ),
        Tile( _bad_index ),
        Tile( _bad_index ),
        Tile( _bad_index ),
        Tile( _gain_two_steel, TileType::EMPTY ),
        Tile( _gain_two_steel, TileType::RESERVED_FOR_OCEAN ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _draw_one_card, TileType::RESERVED_FOR_OCEAN ),
        Tile( _noop, TileType::RESERVED_FOR_OCEAN ),
    }}, // first row
    {{
        Tile( _bad_index ),
        Tile( _bad_index ),
        Tile( _bad_index ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _gain_one_steel, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _draw_two_cards, TileType::RESERVED_FOR_OCEAN ),
    }}, // second row
    {{
        Tile( _bad_index ),
        Tile( _bad_index ),
        Tile( _draw_one_card, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _gain_one_steel, TileType::EMPTY ),
    }}, // third row
    {{
        Tile( _bad_index ),
        Tile( _gain_titanium_and_plants, TileType::EMPTY),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_two_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_two_plants, TileType::RESERVED_FOR_OCEAN ),
    }}, // fourth row
    {{
        Tile( _gain_two_plants, TileType::EMPTY ),
        Tile( _gain_two_plants, TileType::EMPTY ),
        Tile( _gain_two_plants, TileType::RESERVED_FOR_NOCTIS ),
        Tile( _gain_two_plants, TileType::RESERVED_FOR_OCEAN ),
        Tile( _gain_two_plants, TileType::RESERVED_FOR_OCEAN ),
        Tile( _gain_two_plants, TileType::RESERVED_FOR_OCEAN ),
        Tile( _gain_two_plants, TileType::EMPTY ),
        Tile( _gain_two_plants, TileType::EMPTY ),
        Tile( _gain_two_plants, TileType::EMPTY ),
    }}, // fifth row
    {{
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_two_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::RESERVED_FOR_OCEAN ),
        Tile( _gain_one_plants, TileType::RESERVED_FOR_OCEAN ),
        Tile( _gain_one_plants, TileType::RESERVED_FOR_OCEAN ),
        Tile( _bad_index ),
    }}, // sixth row
    {{
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _gain_one_plants, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _bad_index ),
        Tile( _bad_index ),
    }}, // seventh row
    {{
        Tile( _gain_two_steel, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _draw_one_card, TileType::EMPTY ),
        Tile( _draw_one_card, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _gain_one_titanium, TileType::EMPTY ),
        Tile( _bad_index ),
        Tile( _bad_index ),
        Tile( _bad_index ),
    }}, // eighth row
    {{
        Tile( _gain_one_steel, TileType::EMPTY ),
        Tile( _gain_two_steel, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _noop, TileType::EMPTY ),
        Tile( _gain_two_titanium, TileType::RESERVED_FOR_OCEAN ),
        Tile( _bad_index ),
        Tile( _bad_index ),
        Tile( _bad_index ),
        Tile( _bad_index ),
    }}, // ninth row
}};


TharsisConcreteBoard::TharsisIterator::TharsisIterator() :
    _ptr( nullptr ), _q( 0 ), _r( 0 ) {}

TharsisConcreteBoard::TharsisIterator::TharsisIterator( pointer ptr, int q, int r ) :
    _ptr( ptr ), _q( q ), _r( r ) {}

TharsisConcreteBoard::TharsisIterator::~TharsisIterator() {}

ConcreteBoard::Iterator::reference TharsisConcreteBoard::TharsisIterator::operator*() const {
    return *_ptr;
}

ConcreteBoard::Iterator::pointer TharsisConcreteBoard::TharsisIterator::operator->() const {
    return _ptr;
}

ConcreteBoard::Iterator& TharsisConcreteBoard::TharsisIterator::operator++() {
    if ( _r == 8 && _q == 5 )
        return *this;

    PointerOneUp();

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

inline pii TharsisConcreteBoard::TharsisIterator::GetIndices() const {
    return pii( _q, _r );
}

void TharsisConcreteBoard::TharsisIterator::PointerOneUp() {
    ++_ptr;

    if ( _q == 8 ) {
        _q = 0;
        ++_r;
    } else
        ++_q;
}

ConcreteBoard::IteratorWrapper TharsisConcreteBoard::begin() const {
    return new TharsisIterator( &_board[ 0 ][ 4 ], 4, 0 );
}

ConcreteBoard::IteratorWrapper TharsisConcreteBoard::end() const {
    return new TharsisIterator( &_board[ 8 ][ 5 ], 5, 8 );
}
}
