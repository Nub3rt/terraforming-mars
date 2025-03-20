#pragma once

#include <functional>
#include <utility>
#include <vector>

#include "../player.h"
#include "tile.h"

namespace model::boards
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

    inline void SetOnTilePlacedCallback( std::function<void( int, int, const Tile& )> callback ) { _on_tile_placed.SetCallback( callback ); }

protected:
    ConcreteBoard() noexcept;

    Event<int, int, const Tile&> _on_tile_placed;


    static const std::function<void( Player* )>& get_noop();
    static const std::function<void( Player* )>& get_draw_one_card();
    static const std::function<void( Player* )>& get_draw_two_cards();
    static const std::function<void( Player* )>& get_gain_one_plants();
    static const std::function<void( Player* )>& get_gain_two_plants();
    static const std::function<void( Player* )>& get_gain_one_steel();
    static const std::function<void( Player* )>& get_gain_two_steel();
    static const std::function<void( Player* )>& get_gain_one_titanium();
    static const std::function<void( Player* )>& get_gain_two_titanium();

    static const std::function<void( Player* )>& get_gain_titanium_and_plants();

    static const std::function<void( Player* )>& get_bad_index();

public:
    class Iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Tile;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        Iterator();
        virtual ~Iterator();

        virtual reference operator*() const = 0;
        virtual pointer operator->() const = 0;

        virtual Iterator& operator++() = 0;

        virtual bool operator==( const Iterator& other ) const = 0;
        virtual bool operator!=( const Iterator& other ) const = 0;

        virtual pii GetIndices() const = 0;
    };

    class IteratorWrapper : public Iterator
    {
    public:
        IteratorWrapper( Iterator* iterator );
        ~IteratorWrapper();

        reference operator*() const override;
        pointer operator->() const override;

        Iterator& operator++() override;
        bool operator==( const Iterator& other ) const override;
        bool operator!=( const Iterator& other ) const override;

        pii GetIndices() const override;

    private:
        Iterator* _iterator;

    };

    virtual IteratorWrapper begin() const = 0;
    virtual IteratorWrapper end() const = 0;
};
}
