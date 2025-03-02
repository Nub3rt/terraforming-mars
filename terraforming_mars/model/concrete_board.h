#pragma once

#include <functional>
#include <utility>
#include <vector>

#include "player.h"
#include "tile.h"

namespace model::board
{
class ConcreteBoard
{
public:
    using pii = std::pair<int, int>;

    virtual ~ConcreteBoard() noexcept;

    virtual const Tile& get_tile( int q, int r ) const noexcept = 0;

    virtual std::vector<pii> GetNeighbouringTiles( int q, int r ) const = 0;
    virtual std::vector<pii> GetNeighbouringTilesOfType( int q, int r, TileType type ) const;
    virtual std::vector<pii> GetNeighbouringTilesOfTypeRange( int q, int r, TileType min, TileType max ) const;
    virtual const pii* NoctisCityIndex() const;

    virtual void PlaceTile( int q, int r, Player* player, TileType type ) = 0;
    virtual void SetOwner( int q, int r, Player* player ) = 0;
    virtual void SetTileType( int q, int r, TileType type ) = 0;

    inline void SetOnTilePlacedCallback( std::function<void( int, int, const Tile& )> callback );

protected:
    ConcreteBoard() noexcept;

    Event<int, int, const Tile&> _on_tile_placed;


    static const std::function<void( Player* )> _noop;
    static const std::function<void( Player* )> _draw_one_card;
    static const std::function<void( Player* )> _draw_two_cards;
    static const std::function<void( Player* )> _gain_one_plants;
    static const std::function<void( Player* )> _gain_two_plants;
    static const std::function<void( Player* )> _gain_one_steel;
    static const std::function<void( Player* )> _gain_two_steel;
    static const std::function<void( Player* )> _gain_one_titanium;
    static const std::function<void( Player* )> _gain_two_titanium;

    static const std::function<void( Player* )> _gain_titanium_and_plants;

    static const std::function<void( Player* )> _bad_index;
};
}
