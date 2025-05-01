#pragma once

#include <functional>
#include <optional>

#include "../player.hpp"
#include "tile_type.hpp"

namespace model::boards
{
class Tile
{
public:
    Tile( int q, int r, bool volcano, const std::function<void( Player* )>& apply_placement_bonuses, TileType type = TileType::NONE ) noexcept;
    Tile( int q, int r, bool volcano, const std::function<void( Player* )>& apply_placement_bonuses, TileType type, Resource bonus_1 ) noexcept;
    Tile( int q, int r, bool volcano, const std::function<void( Player* )>& apply_placement_bonuses, TileType type, Resource bonus_1, Resource bonus_2 ) noexcept;

    inline TileType get_type() const noexcept { return _type; }
    inline void set_type( TileType type ) noexcept { _type = type; }
    inline Player* get_owner() const noexcept { return _owner; }
    inline void set_owner( Player* owner ) noexcept { _owner = owner; }
    inline bool is_volcano() const noexcept { return _volcano; }
    inline std::pair<int, int> get_indices() const noexcept { return std::pair<int, int>( q, r ); }
    inline const std::optional<Resource>& get_bonus_1() const noexcept { return _bonus_1; }
    inline const std::optional<Resource>& get_bonus_2() const noexcept { return _bonus_2; }
    
    bool IsEmpty() const;

    void ApplyPlacementBonuses( Player* player );

    const int q, r;

private:
    TileType _type;
    bool _volcano;
    Player* _owner;
    const std::function<void( Player* )> _apply_placement_bonuses;
    std::optional<Resource> _bonus_1 = {};
    std::optional<Resource> _bonus_2 = {};
};
}
