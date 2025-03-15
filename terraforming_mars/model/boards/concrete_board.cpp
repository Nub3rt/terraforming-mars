#include "concrete_board.h"

#include <iostream>
#include <vector>
#include <functional>
#include <stdexcept>

#include "../player.h"
#include "../resource.h"
#include "tile_type.h"

namespace model::boards
{
using pii = std::pair<int, int>;

ConcreteBoard::ConcreteBoard() noexcept {}
ConcreteBoard::~ConcreteBoard() noexcept {}

std::vector<pii> ConcreteBoard::GetNeighbouringTilesOfType( int q, int r, TileType type ) const {
    std::vector<pii> neighbours_of_type;

    for ( auto& [q_, r_] : GetNeighbouringTiles( q, r ) ) {
        if ( get_tile( q_, r_ ).get_type() == type )
            neighbours_of_type.emplace_back( q_, r_ );
    }

    return neighbours_of_type;
}

std::vector<pii> ConcreteBoard::GetNeighbouringTilesOfTypeRange( int q, int r, TileType min, TileType max ) const {
    std::vector<pii> neighbours_of_type;

    for ( auto& [q_, r_] : GetNeighbouringTiles( q, r ) ) {
        TileType tile_type = get_tile( q_, r_ ).get_type();
        if ( +tile_type >= +min && +tile_type <= +max )
            neighbours_of_type.emplace_back( q_, r_ );
    }

    return neighbours_of_type;
}

const pii* ConcreteBoard::NoctisCityIndex() const { return nullptr; }

inline void ConcreteBoard::SetOnTilePlacedCallback( std::function<void( int, int, const Tile& )> callback ) {
    _on_tile_placed.SetCallback( callback );
}


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

const std::function<void( Player* )> ConcreteBoard::_bad_index = []( Player* player ) { throw std::logic_error( "ConcreteBoard was indexed on an invalid tile!" ); };


ConcreteBoard::Iterator::Iterator() {}
ConcreteBoard::Iterator::~Iterator() {}

ConcreteBoard::IteratorWrapper::IteratorWrapper( Iterator* iterator ) : _iterator( iterator ){}

ConcreteBoard::IteratorWrapper::~IteratorWrapper() {
    delete _iterator;
}

ConcreteBoard::IteratorWrapper::reference ConcreteBoard::IteratorWrapper::operator*() const {
    return _iterator->operator*();;
}

ConcreteBoard::IteratorWrapper::pointer ConcreteBoard::IteratorWrapper::operator->() const {
    return _iterator->operator->();
}

ConcreteBoard::IteratorWrapper::Iterator& ConcreteBoard::IteratorWrapper::operator++() {
    return _iterator->operator++();
}

bool ConcreteBoard::IteratorWrapper::operator==( const Iterator& other ) const {
    const IteratorWrapper* other_wrapper = dynamic_cast<const IteratorWrapper*>( &other );
    return other_wrapper && _iterator == other_wrapper->_iterator;
}

bool ConcreteBoard::IteratorWrapper::operator!=( const Iterator& other ) const {
    return !(*this == other);
}

pii ConcreteBoard::IteratorWrapper::GetIndices() const {
    return _iterator->GetIndices();
}
}
