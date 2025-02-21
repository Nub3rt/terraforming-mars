#pragma once

#include <vector>
#include <functional>

#include "player.h"
#include "tile.h"

namespace model::board
{
class ConcreteBoard
{
public:
    virtual ~ConcreteBoard() noexcept;

    virtual const Tile& operator()( int q, int r ) const noexcept = 0;

    bool CanPlaceTile( int q, int r, const Player* player, TileType type ) const;
    void PlaceTile( int q, int r, Player* player, TileType type );
    int NeighbouringTiles( int q, int r ) const;
    int NeighbouringTilesOfType( int q, int r, TileType type ) const;

protected:
    ConcreteBoard() noexcept;

    virtual Tile& get_tile( int q, int r ) noexcept = 0;

    virtual std::vector<std::reference_wrapper<const Tile>> GetNeighbours( int q, int r ) const = 0;

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

    static const std::function<void( Player* )> _warn_bad_index;
};
}
