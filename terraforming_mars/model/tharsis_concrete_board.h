#pragma once

#include <array>
#include <functional>

#include "concrete_board.h"
#include "hexagonal_grid.h"
#include "player.h"
#include "tile.h"

namespace model::board
{
class TharsisConcreteBoard : public ConcreteBoard
{
public:
    using pii = std::pair<int, int>;

    TharsisConcreteBoard() noexcept;
    ~TharsisConcreteBoard() noexcept;

    inline Tile& get_tile( int q, int r ) noexcept;

    std::vector<pii> GetNeighbouringTiles( int q, int r ) const override;
    const pii* NoctisCityIndex() const override;

    void PlaceTile( int q, int r, Player* player, TileType type ) override;
    void SetOwner( int q, int r, Player* player ) override;
    void SetTileType( int q, int r, TileType type ) override;

protected:
    std::array<std::array<Tile, 9>, 9> _board;

    static const pii _noctis_city_index;
    static const std::array<std::array<Tile, 9>, 9> _starting_board;

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

        inline pii GetIndices() const override;

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
