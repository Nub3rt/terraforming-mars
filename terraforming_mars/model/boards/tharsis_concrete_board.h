#pragma once

#include <array>
#include <functional>

#include "concrete_board.h"
#include "hexagonal_grid.h"
#include "../player.h"
#include "tile.h"

namespace model::boards
{
class TharsisConcreteBoard : public ConcreteBoard
{
public:
    using pii = std::pair<int, int>;

    TharsisConcreteBoard() noexcept;
    ~TharsisConcreteBoard() noexcept;

    inline Tile& get_tile( int q, int r ) noexcept { return _board[ r ][ q ]; }
    const Tile& get_tile( int q, int r ) const noexcept override;

    std::vector<pii> GetNeighbouringTiles( int q, int r ) const override;
    const pii* NoctisCityIndex() const override;

    void PlaceTile( int q, int r, Player* player, TileType type ) override;
    void SetOwner( int q, int r, Player* player ) override;
    void SetTileType( int q, int r, TileType type ) override;

protected:
    std::array<std::array<Tile, 9>, 9> _board;

    static const pii _noctis_city_index;
    static const std::array<std::array<Tile, 9>, 9>& get_starting_board();

public:
    class TharsisIterator : public ConcreteBoard::Iterator
    {
    public:
        TharsisIterator();
        TharsisIterator( pointer ptr, int q, int r );
        ~TharsisIterator() override;

        reference operator*() const override;
        pointer operator->() const override;

        Iterator& operator++() override;

        bool operator==( const Iterator& other ) const override;
        bool operator!=( const Iterator& other ) const override;

        inline pii GetIndices() const override { return pii( _q, _r ); }

        void PointerOneUp();

    private:
        pointer _ptr;
        int _q;
        int _r;
    };

    IteratorWrapper begin() const override;
    IteratorWrapper end() const override;
};
}
