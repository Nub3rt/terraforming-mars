#pragma once

namespace model::board
{
template <typename T>
class HexagonalGrid
{
public:
    HexagonalGrid() noexcept {}

    inline const T& operator()( int q, int r ) const noexcept { return _grid[ r ][ q ]; }
    inline T& operator()( int q, int r ) noexcept { return _grid[ r ][ q ]; }
private:
    T _grid[ 9 ][ 9 ];
};
}
